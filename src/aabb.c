#include "aabb.h"

bool AABB_intersectsAABB(AABB *a, AABB *b)
{
    return (a->minX <= b->maxX && a->maxX >= b->minX) &&
           (a->minY <= b->maxY && a->maxY >= b->minY);
}

AABB AABB_fromGameObject(GameObject *gameObject)
{
    AABB aabb;
    aabb.minX = gameObject->x;
    aabb.minY = gameObject->y;
    aabb.maxX = gameObject->x + TILE_SIZE;
    aabb.maxY = gameObject->y + TILE_SIZE;
    return aabb;
}
