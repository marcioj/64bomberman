#ifndef AABB_H
#define AABB_H

#include <stdbool.h>
#include "constants.h"
#include "game_object.h"

typedef struct AABB
{
    float minX;
    float minY;
    float maxX;
    float maxY;
} AABB;

bool AABB_intersectsAABB(AABB *a, AABB *b);

AABB AABB_fromGameObject(GameObject *gameObject);

#endif