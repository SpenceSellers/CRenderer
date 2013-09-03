#ifndef OBJECTS_H
#define OBJECTS_H
#include "vectors.h"

#include <SDL/SDL.h>
/* A model is a shape (series of lines) with no positional information at all. */
typedef struct Model {
    Line3D line;
    struct Model *next;
} Model;

typedef struct Object {
    Point3D pos;
    Model *model;
    Uint32 color;
    RotationVector rotation;
} Object;

/* Scales a model by a factor.
   Note that this scales away from the model's personal origin,
   not the center of the model and definitely not the global origin.

*/
void Model_scale(Model *model, double factor);

// Returns the new model pointer!
Model * Model_add(Model *model, Line3D line);

// Creates a new object according to the common parameters.
Object * Object_new(Model *model, Uint32 color, int x, int y, int z);


// Applies a rotation delta to an object.
void Object_rotate(Object *obj, float rotX, float rotY, float rotZ);
#endif
