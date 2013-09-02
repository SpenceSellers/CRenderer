#ifndef SCENE_H
#define SCENE_H

#include "objects.h"
#include "vectors.h"

typedef struct SceneList {
    Object *obj;
    struct SceneList *next;
} SceneList;

typedef struct Scene {
    SceneList *objects;
} Scene;

void Scene_add_object(Scene *scene, Object *obj);

void Scene_remove_object(Scene *scene, Object *obj);


#endif
