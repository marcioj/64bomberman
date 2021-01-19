#ifndef COLLISION_H
#define COLLISION_H

#include "game_object.h"

typedef struct CollisionResult
{
    bool down;
    bool up;
    bool left;
    bool right;
} CollisionResult;

bool checkCollisionRight(float x, float y, CollisionResult *result);

bool checkCollisionLeft(float x, float y, CollisionResult *result);

bool checkCollisionUp(float x, float y, CollisionResult *result);

bool checkCollisionDown(float x, float y, CollisionResult *result);

bool handleCollision(GameObject *gameObject);

#endif
