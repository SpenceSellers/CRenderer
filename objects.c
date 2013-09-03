#include "objects.h"

void Model_scale(Model *model, double factor){
    Model *current;
    for (current = model; current != NULL; current= current->next){
	//Line3D *l;
        Line3D_scale(&current->line, factor);
    }
}

Model * Model_add(Model *model, Line3D line){
    printf("Added line: %d %d %d -> %d %d %d\n",
	   line.p1.x, line.p1.y, line.p1.z,
	   line.p2.x, line.p2.y, line.p2.z);
    Model *new_node = malloc(sizeof(Model));
    
    new_node->line = line;
    new_node->next = model;
    return new_node;
}

void Model_print(Model *model){
    printf("Model: {\n");
    Model *current;
    for (current = model; current != NULL; current = current->next){
	Line3D line = current->line;
	printf("Line: %d %d %d -> %d %d %d\n",
	       line.p1.x, line.p1.y, line.p1.z,
	       line.p2.x, line.p2.y, line.p2.z);
    }
    printf("} \n");
}
Object * Object_new(Model *model, Uint32 color, int x, int y, int z){
    printf("Model at %p\n", model);
    Model_print(model);
    Object *obj = malloc(sizeof(Object));
    obj->model = model;
    obj->color = color;

    Point3D pos;
    pos.x = x;
    pos.y = y;
    pos.z = z;

    obj->pos = pos;

    obj->ai = NULL;
    obj->ai_data_free = NULL;
    // We're going to assume that the rotation starts at 0.
    // The caller can change it later if they want.
    RotationVector rot = {0.0,0.0,0.0};
    obj->rotation = rot;
    
    return obj;
}

void Object_rotate(Object *obj, float rotX, float rotY, float rotZ){
    obj->rotation.rotX += rotX;
    obj->rotation.rotY += rotY;
    obj->rotation.rotZ += rotZ;
}

void Object_tick(Object *obj, Scene *scene){
    if (obj->ai == NULL) return;
    (obj->ai)(obj, scene);
}

    
    
