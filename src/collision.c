#include <stdbool.h>
#include "collision.h"
#include "constants.h"
#include "level.h"
#include "grid.h"

static float slideThreshold = 0.4f;

bool checkCollisionRight(float x, float y, CollisionResult *result)
{
    int upperLimitX = Level_getWidth() - (TILE_SIZE * 2);
    if (x > upperLimitX)
        return true;
    int right = Grid_coordToGrid(x) + 1;
    int top = Grid_coordToGrid(y);
    int bottom = Grid_coordToGrid(y) + 0.9f;
    bool topCol = Grid_hasCollision(right, top);
    bool bottomCol = Grid_hasCollision(right, bottom);

    if (result)
    {
        memset(result, 0, sizeof(CollisionResult));
        result->up = topCol;
        result->down = bottomCol;
    }

    return topCol || bottomCol;
}

bool checkCollisionLeft(float x, float y, CollisionResult *result)
{
    if (x < TILE_SIZE)
        return true;
    int left = Grid_coordToGrid(x);
    int top = Grid_coordToGrid(y);
    int bottom = Grid_coordToGrid(y) + 0.9f;
    bool topCol = Grid_hasCollision(left, top);
    bool bottomCol = Grid_hasCollision(left, bottom);

    if (result)
    {
        memset(result, 0, sizeof(CollisionResult));
        result->up = topCol;
        result->down = bottomCol;
    }

    return topCol || bottomCol;
}

bool checkCollisionUp(float x, float y, CollisionResult *result)
{
    if (y < TILE_SIZE)
        return true;
    int left = Grid_coordToGrid(x);
    int top = Grid_coordToGrid(y);
    int right = Grid_coordToGrid(x) + 0.9f;
    bool leftCol = Grid_hasCollision(left, top);
    bool rightCol = Grid_hasCollision(right, top);

    if (result)
    {
        memset(result, 0, sizeof(CollisionResult));
        result->left = leftCol;
        result->right = rightCol;
    }

    return leftCol || rightCol;
}

bool checkCollisionDown(float x, float y, CollisionResult *result)
{
    int upperLimitY = Level_getHeight() - (TILE_SIZE * 2 + 1);
    if (y > upperLimitY)
        return true;
    int left = Grid_coordToGrid(x);
    int bottom = Grid_coordToGrid(y) + 1;
    int right = Grid_coordToGrid(x) + 0.9f;
    bool leftCol = Grid_hasCollision(left, bottom);
    bool rightCol = Grid_hasCollision(right, bottom);

    if (result)
    {
        memset(result, 0, sizeof(CollisionResult));
        result->left = leftCol;
        result->right = rightCol;
    }

    return leftCol || rightCol;
}

bool handleCollision(GameObject *gameObject)
{
    float x = gameObject->x;
    float y = gameObject->y;
    bool anyCollision = false;
    bool movingRight = gameObject->x > gameObject->prevX;
    bool movingLeft = gameObject->x < gameObject->prevX;
    bool movingDown = gameObject->y > gameObject->prevY;
    bool movingUp = gameObject->y < gameObject->prevY;

    if (movingRight)
    {
        CollisionResult result;
        if (checkCollisionRight(x, y, &result))
        {
            // gameObject->x = (int)(x / TILE_SIZE) * TILE_SIZE;
            gameObject->x = gameObject->prevX;
            anyCollision = true;

            // make player slide through gaps
            if (result.up ^ result.down)
            {
                float y = gameObject->y / TILE_SIZE;
                y -= (int)y;
                if (result.up && y > 1.0f - slideThreshold)
                    gameObject->y++;
                if (result.down && y < slideThreshold)
                    gameObject->y--;
            }
        }
    }
    else if (movingLeft)
    {
        CollisionResult result;
        if (checkCollisionLeft(x, y, &result))
        {
            // gameObject->x = ((int)(x / TILE_SIZE) + 1) * TILE_SIZE;
            gameObject->x = gameObject->prevX;
            anyCollision = true;

            // make player slide through gaps
            if (result.up ^ result.down)
            {
                float y = gameObject->y / TILE_SIZE;
                y -= (int)y;
                if (result.up && y > 1.0f - slideThreshold)
                    gameObject->y++;
                if (result.down && y < slideThreshold)
                    gameObject->y--;
            }
        }
    }

    if (movingDown)
    {
        CollisionResult result;
        if (checkCollisionDown(x, y, &result))
        {
            // gameObject->y = (int)(y / TILE_SIZE) * TILE_SIZE;
            gameObject->y = gameObject->prevY;
            anyCollision = true;

            // make player slide through gaps
            if (result.left ^ result.right)
            {
                float x = gameObject->x / TILE_SIZE;
                x -= (int)x;
                if (result.left && x > 1.0f - slideThreshold)
                    gameObject->x++;
                if (result.right && x < slideThreshold)
                    gameObject->x--;
            }
        }
    }
    else if (movingUp)
    {
        CollisionResult result;
        if (checkCollisionUp(x, y, &result))
        {
            // gameObject->y = ((int)(y / TILE_SIZE) + 1) * TILE_SIZE;
            gameObject->y = gameObject->prevY;
            anyCollision = true;

            // make player slide through gaps
            if (result.left ^ result.right)
            {
                float x = gameObject->x / TILE_SIZE;
                x -= (int)x;
                if (result.left && x > 1.0f - slideThreshold)
                    gameObject->x++;
                if (result.right && x < slideThreshold)
                    gameObject->x--;
            }
        }
    }
    return anyCollision;
}
