#include "scene.h"

void Scene_add_object(Scene *scene, Object *obj){
    SceneList *new = malloc(sizeof(SceneList));
    new->obj = obj;
    new->next = scene->objects;
    scene->objects = new;
}


void Scene_remove_object(Scene *scene, Object *obj){
    SceneList *last = NULL;
    SceneList *current = scene->objects;

    while (current != NULL){
	if (current->obj == obj){
	    if (last == NULL){
		scene->objects = current->next;
	    } else {
		last->next = current->next;
	    }
	    current->next = NULL;
	    return;
	}
	last = current;
	current = current->next;
    }
}
