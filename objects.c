#include "objects.h"

void Model_scale(Model *model, double factor){
    int i;
    for (i = 0; i < model->num_lines; i++){
	Line3D *l;
        Line3D_scale(&model->lines[i], factor);
    }
}


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

void Object_rotate(Object *obj, float rotX, float rotY, float rotZ){
    obj->rotation.rotX += rotX;
    obj->rotation.rotY += rotY;
    obj->rotation.rotZ += rotZ;
}
