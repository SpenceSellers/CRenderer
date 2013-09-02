#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <SDL/SDL.h>

#include "vectors.h"

#define HEIGHT 800
#define WIDTH 800
#define MOVESPEED 10
#define MOUSESCALING 1.0/1000

/* A model is a shape (series of lines) with no positional information at all. */
typedef struct Model {
    int num_lines;
    Line3D *lines;
} Model;

typedef struct Object {
    Point3D pos;
    Model *model;
    Uint32 color;
    RotationVector rotation;
} Object;

// Sets a pixel at a point on a surface to a color.
void set_pixel(SDL_Surface *surf, int x, int y, Uint32 color){
    if (x >= surf->w || x < 0){
	return;
    }
    if (y >= surf->h || y < 0){
	return;
    }
    Uint32 * pixmem32;
    pixmem32 = (Uint32*) surf->pixels + (y * surf->w) + x;
    *pixmem32 = color;
}

/* Draws a line on a surface using Bresenham's line algorithm */
void draw_line(SDL_Surface *surf, int x1, int y1, int x2, int y2, Uint32 color){
    // Bresenham's line algorithm
    
    int dx = abs(x2-x1);
    int sx = x1<x2 ? 1 : -1;
    int dy = abs(y2-y1);
    int sy = y1<y2 ? 1 : -1; 
    int err = (dx>dy ? dx : -dy)/2;
    int e2;
 
    for(;;){
	set_pixel(surf, x1, y1, color);
	if (x1==x2 && y1==y2) break;
	e2 = err;
	if (e2 >-dx) { err -= dy; x1 += sx; }
	if (e2 < dy) { err += dx; y1 += sy; }
    }
}



/* Projects a 3D point onto a 2D 'surface'.
   The camera is always at (0,0,0), so if the camera isn't actually
   there you'll need to move the points themselves.
 */
Point2D project_point(Point3D point, int eye_distance){
    Point2D s;
    if (point.z <= 0){ //Point is behind camera
	s.x = 0;
	s.y = 0;
	return s;
    }
    s.x = ((point.x * eye_distance) / (point.z)) + WIDTH/2;
    s.y = ((point.y * eye_distance) / (point.z)) + HEIGHT/2;

    return s;
}

void render_line3D(Line3D l, Point3D offset,  SDL_Surface *surf, Uint32 color, RotationVector scenerotation){
    //Point3D origin = {0,0,0};
    //Line3D rotated = Line3D_rotate_around_point(l, scenerotation, origin);
    Point3D first = Point3D_rotate(Point3D_add(l.p1, offset), scenerotation);
    Point3D second = Point3D_rotate(Point3D_add(l.p2, offset), scenerotation);
    //Project the two points.
    Point2D p1 = project_point(first, 500);
    Point2D p2 = project_point(second, 500);
    draw_line(surf, p1.x, p1.y, p2.x, p2.y, color);
}

void render_model(SDL_Surface *surf,
		  Model *model,
		  Point3D offset,
		  RotationVector scenerotation,
		  Point3D scenerotation_center,
		  Uint32 color,
		  RotationVector objrotation,
		  Point3D rotcenter){
    int i;
    Point3D origin = {0,0,0};
    for(i = 0; i < model->num_lines; i++){ // For every line in the model... 
	Line3D line = Line3D_rotate_around_point(model->lines[i], objrotation, rotcenter); 
	//line = Line3D_rotate_around_point(line, scenerotation, offset);
	render_line3D(line, offset, surf, color, scenerotation);
    }
}

void render_object(SDL_Surface *surf, Object *obj, Point3D offset, RotationVector scenerotation){
    Point3D origin = {0,0,0};
    render_model(surf,
		 obj->model, //Model
		 Point3D_add(obj->pos,offset), //Offset
		 scenerotation,
		 origin, //Scene Rotation Center
		 obj->color,
		 obj->rotation, //Object rotation
		 Point3D_rotate_around_point(obj->pos, scenerotation, origin) //Object rotation center
	); 
}

/* Scales a model by a factor.
   Note that this scales away from the model's personal origin,
   not the center of the model and definitely not the global origin.

 */
void Model_scale(Model *model, double factor){
    int i;
    for (i = 0; i < model->num_lines; i++){
	Line3D *l;
        Line3D_scale(&model->lines[i], factor);
    }
}

// Creates a new object according to the common parameters.
Object * Object_new(Model *model, Uint32 color, int x, int y, int z){
    Object *obj = malloc(sizeof(Object));
    obj->model = model;
    obj->color = color;

    Point3D pos;
    pos.x = x;
    pos.y = y;
    pos.z = z;

    obj->pos = pos;
    
    // We're going to assume that the rotation starts at 0.
    // The caller can change it later if they want.
    RotationVector rot = {0.0,0.0,0.0};
    obj->rotation = rot;
    
    return obj;
}

// Applies a rotation delta to an object.
void Object_rotate(Object *obj, float rotX, float rotY, float rotZ){
    obj->rotation.rotX += rotX;
    obj->rotation.rotY += rotY;
    obj->rotation.rotZ += rotZ;
   
}

//Generates a new model of a 1x1x1 cube. Will probably need to be scaled.
Model * generate_cube(){
    Model *model = malloc(sizeof(Model));
    model->num_lines = 12;
    model->lines = calloc(12, sizeof(Line3D));
    
    model->lines[0] = (Line3D) {0,0,0,0,0,1};
    model->lines[1] = (Line3D) {0,1,0,0,1,1};
    model->lines[2] = (Line3D) {1,1,0,1,1,1};
    model->lines[3] = (Line3D) {1,0,0,1,0,1};

    model->lines[4] = (Line3D) {0,0,0,1,0,0};
    model->lines[5] = (Line3D) {0,0,1,1,0,1};
    model->lines[6] = (Line3D) {0,1,0,1,1,0};
    model->lines[7] = (Line3D) {0,1,1,1,1,1};

    model->lines[8] = (Line3D) {0,0,0,0,1,0};
    model->lines[9] = (Line3D) {1,0,0,1,1,0};
    model->lines[10] = (Line3D) {1,0,1,1,1,1};
    model->lines[11] = (Line3D) {0,0,1,0,1,1};

    return model;
}
    
    
int main(){
    SDL_Surface *screen; 
    SDL_Event event;
    SDL_Init(SDL_INIT_VIDEO);
    screen = SDL_SetVideoMode(HEIGHT, WIDTH, 32, SDL_RESIZABLE | SDL_HWSURFACE);
    
    // Set up mouse.
    SDL_ShowCursor(0);
    SDL_WM_GrabInput(SDL_GRAB_ON);
    
    int running = 1;
    int time = 0;
    
    Point3D offset = {0,0,0};
    RotationVector scene_rotation = {0.0,0.0,0.0};

    double lookX = 0.0;
    double lookY = 0.0;
    
    Model *cubeModel = generate_cube(100);
    Model_scale(cubeModel, 100);
    Object *cube1 = Object_new(cubeModel, 0xFF0000, 0,0,0);
    Object *cube2 = Object_new(cubeModel, 0x00FF00, 100,0,800);
    Object *cube3 = Object_new(cubeModel, 0x0000FF, -400, 0, 0);
    
    Uint8 *keyState = SDL_GetKeyState(NULL);
    while (running == 1){
	time++;
	SDL_FillRect(screen, NULL, 0x000000); //Clear the screen
	render_object(screen, cube1, offset, scene_rotation);
	render_object(screen, cube2, offset, scene_rotation);
	render_object(screen, cube3, offset, scene_rotation);
	
	SDL_Flip(screen);
	if (time % 100 == 0){
	    printf("Scene: %lf, %lf \n", cos(scene_rotation.rotY), sin(scene_rotation.rotY));
	    printf("Look: %lf, %lf \n", cos(lookX), sin(lookX));
	}
	// Handle Events
	while (SDL_PollEvent(&event)){
	    switch (event.type){
	    case SDL_QUIT:
		running = 0;
		break;

	    case SDL_MOUSEMOTION:
		lookX -= event.motion.xrel * MOUSESCALING;
		lookY += event.motion.yrel * MOUSESCALING;
		
		scene_rotation.rotY = lookX;
		// Since the camera is always looking in the same direction from a global standpoint,
		// it is fine to just adjust the X rotation of the scene.
		scene_rotation.rotX = lookY;
		
		
	    }
	    
	}
	if (keyState[SDLK_w]){
	    offset.z -= MOVESPEED;
	}
	if (keyState[SDLK_s]){
	    offset.z += MOVESPEED;
	}
	if (keyState[SDLK_a]){
	    offset.x += MOVESPEED;
	}
	if (keyState[SDLK_d]){
	    offset.x -= MOVESPEED;
	}
	if (keyState[SDLK_COMMA]){
	    offset.y -= MOVESPEED;
	}
	if (keyState[SDLK_PERIOD]){
	    offset.y += MOVESPEED;
	}
	if (keyState[SDLK_SEMICOLON]){
	    scene_rotation.rotZ += 0.01;
	}
	if (keyState[SDLK_QUOTE]){
	    scene_rotation.rotZ -= 0.01;
	}
	if (keyState[SDLK_ESCAPE]){
	    if (SDL_WM_GrabInput(SDL_GRAB_QUERY) == SDL_GRAB_ON){
		printf("Releasing mouse...\n");
		SDL_WM_GrabInput(SDL_GRAB_OFF);
		SDL_ShowCursor(1);
		SDL_Delay(100); // Let the user stop pressing escape.
	    } else {
		printf("Grabbing mouse...\n");
		SDL_WM_GrabInput(SDL_GRAB_ON);
		SDL_ShowCursor(0);
	    }
	}
	SDL_Delay(10);
    }
    SDL_Quit();
    return 0;
}
