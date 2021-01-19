#ifndef WORLD_H
#define WORLD_H

#include "game_object.h"
#include "aabb.h"

typedef struct World
{
    GameObjectArray gameObjects;
    GameObjectArray gameObjectsToDestroy;
} World;

extern World *world;

World *World_new();

void World_update(World *world, float dt);

void World_addGameObject(World *world, GameObject *gameObject);

void World_removeGameObject(World *world, GameObject *gameObject);

void World_render(World *world);

void World_clear(World *world);

#endif
