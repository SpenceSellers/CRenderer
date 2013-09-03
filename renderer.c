#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <SDL/SDL.h>

#include "vectors.h"
#include "objects.h"
#include "scene.h"

#define HEIGHT 800
#define WIDTH 800
#define MOVESPEED 10
#define MOUSESCALING 1.0/1000


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

    Point3D origin = {0,0,0};
    Model *current;
    
    for(current = model; current != NULL; current = current->next){
        // For every line in the model...
	Line3D line = Line3D_rotate_around_point(current->line,
						 objrotation,
						 origin); 
	//line = Line3D_rotate_around_point(line, scenerotation, offset);
	render_line3D(line, offset, surf, color, scenerotation);
    }
}

void render_object(SDL_Surface *surf,
		   Object *obj,
		   Point3D offset,
		   RotationVector scenerotation){
    Point3D origin = {0,0,0};
    render_model(surf,
		 obj->model, //Model
		 Point3D_add(obj->pos,offset), //Offset
		 scenerotation,
		 origin, //Scene Rotation Center
		 obj->color,
		 obj->rotation, //Object rotation
		 obj->pos
		 //Point3D_rotate_around_point(obj->pos, scenerotation, obj->pos) //Object rotation center
		 ); 
}

void render_scene(SDL_Surface *surf, Scene *scene, Point3D offset, RotationVector scenerotation){
    SceneList *current;
    for (current = scene->objects; current != NULL; current = current->next){
	render_object(surf, current->obj, offset, scenerotation);
    }
}


void rotateAI(Object *obj, Scene *scene){
    obj->rotation.rotY += M_PI/100;
}
//Generates a new model of a 1x1x1 cube. Will probably need to be scaled.
Model * generate_cube(){
    Model *model;
    model = Model_add(NULL, (Line3D) {0,0,0,0,0,1});
    model = Model_add(model, (Line3D) {0,1,0,0,1,1});
    model = Model_add(model, (Line3D) {1,1,0,1,1,1});
    model = Model_add(model, (Line3D) {1,0,0,1,0,1});
    model = Model_add(model, (Line3D) {0,0,0,1,0,0});
    model = Model_add(model, (Line3D) {0,0,1,1,0,1});
    model = Model_add(model, (Line3D) {0,1,0,1,1,0});
    model = Model_add(model, (Line3D) {0,1,1,1,1,1});

    model = Model_add(model, (Line3D) {0,0,0,0,1,0});
    model = Model_add(model, (Line3D) {1,0,0,1,1,0});
    model = Model_add(model, (Line3D) {1,0,1,1,1,1});
    model = Model_add(model, (Line3D) {0,0,1,0,1,1});
    printf("Model at %p\n", model);
    printf("Model->next %p\n", model->next);
    return model;
}

Model * gen_newcube(){
    Model *model;
    model = Model_add(NULL, (Line3D) {0,0,0,0,0,100});
    model = Model_extrude(model, (Point3D) {0,100,0},1);
    model = Model_extrude(model, (Point3D) {100, 0, 0},1);
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

    Scene *scene = Scene_new();
    
    Point3D offset = {0,0,0};
    RotationVector scene_rotation = {0.0,0.0,0.0};

    double lookX = 0.0;
    double lookY = 0.0;
    //Model *cubeModel = generate_cube(100);
    //Model_scale(cubeModel, 100);
    Model *cubeModel = gen_newcube();
    Model_centerize(cubeModel);
    Scene_add_object(scene, Object_new(cubeModel, 0xFF0000, 0,0,0));
    Scene_add_object(scene, Object_new(cubeModel, 0x00FF00, 100,0,800));
    Scene_add_object(scene, Object_new(cubeModel, 0x0000FF, -400, 0, 0));

    scene->objects->obj->ai = &rotateAI;
    
    Uint8 *keyState = SDL_GetKeyState(NULL);
    while (running == 1){
	time++;
	Scene_tick(scene);
	SDL_FillRect(screen, NULL, 0x000000); //Clear the screen

	render_scene(screen, scene, offset, scene_rotation);
	
	SDL_Flip(screen);
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
	    offset.z -= cos(scene_rotation.rotY) * MOVESPEED;
	    offset.x += sin(scene_rotation.rotY) * MOVESPEED;
	}
	if (keyState[SDLK_s]){
	    offset.z += cos(scene_rotation.rotY) * MOVESPEED;
	    offset.x -= sin(scene_rotation.rotY) * MOVESPEED;
	}
	if (keyState[SDLK_a]){
	    offset.x += cos(scene_rotation.rotY) * MOVESPEED;
	    offset.z += sin(scene_rotation.rotY) * MOVESPEED;
	}
	if (keyState[SDLK_d]){
	    offset.x -= cos(scene_rotation.rotY) * MOVESPEED;
	    offset.z -= sin(scene_rotation.rotY) * MOVESPEED;
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
	if (keyState[SDLK_f]){
	    Scene_add_object(scene, Object_new(cubeModel, 0xFF00FF, -400, 0, 1000));
	}
	if (keyState[SDLK_ESCAPE]){
	    if (SDL_WM_GrabInput(SDL_GRAB_QUERY) == SDL_GRAB_ON){
		printf("Releasing mouse...\n");
		SDL_WM_GrabInput(SDL_GRAB_OFF);
		SDL_ShowCursor(1);
		SDL_Delay(1000); // Let the user stop pressing escape.
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
