#ifndef SCENE_H
#define SCENE_H

//Some forward declarations
typedef struct Object Object;
typedef struct Scene Scene;
#include "objects.h"
#include "vectors.h"

typedef struct SceneList {
    Object *obj;
    struct SceneList *next;
} SceneList;

typedef struct Scene {
    SceneList *objects;
    int time;
} Scene;

Scene * Scene_new();

void Scene_add_object(Scene *scene, Object *obj);

void Scene_remove_object(Scene *scene, Object *obj);

void Scene_tick(Scene *scene);

#endif
