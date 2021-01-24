#include "world.h"
#include "constants.h"
#include "aabb.h"
#include "math_utils.h"
#include <vec.h>
#include <malloc.h>
#include "screen_printf.h"

World *World_new()
{
    World *world = malloc(sizeof(World));
    vec_init(&world->gameObjects);
    vec_reserve(&world->gameObjects, GAME_OBJECT_MAX_COUNT);
    vec_init(&world->gameObjectsToDestroy);
    vec_reserve(&world->gameObjectsToDestroy, GAME_OBJECT_MAX_COUNT);
    return world;
}

static int sortByZIndex(const void *a, const void *b)
{
    GameObject *objA = *(GameObject **)a;
    GameObject *objB = *(GameObject **)b;
    if (objA->zIndex > objB->zIndex)
        return 1;
    else if (objA->zIndex < objB->zIndex)
        return -1;
    else
        return 0;
}

void World_addGameObject(World *world, GameObject *gameObject)
{
    vec_push(&world->gameObjects, gameObject);
    vec_sort(&world->gameObjects, sortByZIndex);
}

void World_destroyScheduledGameObjects(World *world)
{
    for (size_t i = 0; i < world->gameObjectsToDestroy.length; i++)
    {
        GameObject *destroyed = world->gameObjectsToDestroy.data[i];
        vec_remove(&world->gameObjects, destroyed);
        GameObject_destroy(destroyed);
    }
    vec_clear(&world->gameObjectsToDestroy);
}

void World_removeGameObject(World *world, GameObject *gameObject)
{
    vec_push(&world->gameObjectsToDestroy, gameObject);
}

void World_updateGameObjects(World *world, float dt)
{
    for (size_t i = 0; i < world->gameObjects.length; i++)
    {
        GameObject *gameObject = world->gameObjects.data[i];
        GameObject_update(gameObject, dt);
    }
}

void World_render(World *world)
{
    for (size_t i = 0; i < world->gameObjects.length; i++)
    {
        GameObject *gameObject = world->gameObjects.data[i];
        gameObject->render(gameObject);
    }
}

static void handleGameObjectCollision(World *world)
{
    if (!world->gameObjects.length)
        return;
    // collision enter
    for (size_t i = 0; i < world->gameObjects.length - 1; i++)
    {
        GameObject *gameObjectA = world->gameObjects.data[i];
        AABB boundsA = AABB_fromGameObject(gameObjectA);
        // make bounds smaller to prevent precision issues
        boundsA.minX++;
        boundsA.minY++;
        boundsA.maxX--;
        boundsA.maxY--;

        for (size_t j = i + 1; j < world->gameObjects.length; j++)
        {
            GameObject *gameObjectB = world->gameObjects.data[j];
            if (gameObjectA == gameObjectB)
                continue;
            AABB boundsB = AABB_fromGameObject(gameObjectB);
            if (AABB_intersectsAABB(&boundsA, &boundsB))
            {
                vec_push(gameObjectA->_newCollisions, gameObjectB);
                vec_push(gameObjectB->_newCollisions, gameObjectA);
                if (gameObjectA->onCollisionEnter)
                    gameObjectA->onCollisionEnter(gameObjectA, gameObjectB);
                if (gameObjectB->onCollisionEnter)
                    gameObjectB->onCollisionEnter(gameObjectB, gameObjectA);
            }
        }
        // collision exit
        for (size_t k = 0; k < gameObjectA->_prevCollisions->length; k++)
        {
            GameObject *prevCollision = gameObjectA->_prevCollisions->data[k];
            int found = -1;
            vec_find(gameObjectA->_newCollisions, prevCollision, found);
            if (found == -1)
            {
                if (prevCollision->onCollisionExit)
                    prevCollision->onCollisionExit(prevCollision, gameObjectA);
                if (gameObjectA->onCollisionExit)
                    gameObjectA->onCollisionExit(gameObjectA, prevCollision);
            }
        }
    }

    // assign newCollisions to prevCollisions
    for (size_t i = 0; i < world->gameObjects.length; i++)
    {
        GameObjectArray *prevCollisions = world->gameObjects.data[i]->_prevCollisions;
        GameObjectArray *newCollisions = world->gameObjects.data[i]->_newCollisions;
        vec_reserve(prevCollisions, newCollisions->length);
        prevCollisions->length = newCollisions->length;
        for (size_t j = 0; j < newCollisions->length; j++)
        {
            prevCollisions->data[j] = newCollisions->data[j];
        }
        vec_clear(newCollisions);
    }
}

void World_update(World *world, float dt)
{
    World_updateGameObjects(world, dt);
    handleGameObjectCollision(world);
    World_destroyScheduledGameObjects(world);
}

void World_clear(World *world)
{
    for (size_t i = 0; i < world->gameObjects.length; i++)
    {
        GameObject *obj = world->gameObjects.data[i];
        int found = -1;
        vec_find(&world->gameObjectsToDestroy, obj, found);
        if (found == -1)
            vec_push(&world->gameObjectsToDestroy, obj);
    }
}
