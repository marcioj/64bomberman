#include "block.h"
#include "constants.h"
#include "screen_printf.h"
#include "world.h"
#include "camera.h"
#include "math_utils.h"
#include "level.h"
#include "textures.h"
#include "grid.h"

Animation *defaultAnimation;

Animation *createAnimation()
{
    GameObject *gameObject = GameObject_new();
    gameObject->zIndex = 2;
    Animation *animation = Animation_new();
    gameObject->animations = animation;
    animation->gameObject = gameObject;
    animation->texture = spritesheet;
    animation->speed = 0.07f;
    animation->spriteCol = 4;
    animation->spriteRow = 3;
    animation->steps = 7;
    animation->defaultStep = 0;
    animation->iterationCount = 1;
    animation->automatic = true;
    return animation;
}

void Block_init()
{
    defaultAnimation = createAnimation();
}

void Block_generateRandom()
{
    int blockCount = 50;

    while (true)
    {
        for (size_t col = 0; col < GRID_COLS; col++)
        {
            for (size_t row = 0; row < GRID_ROWS; row++)
            {
                if (blockCount == 0)
                    return;
                // do not trap the player around blocks
                if ((col == 0 && row == 0) || (col == 1 && row == 0) || (col == 0 && row == 1))
                    continue;
                // only put block in valid places
                if (Grid_hasCollision(col, row))
                    continue;

                if (randomMinMax(0, 9) == 0)
                {
                    Grid_setCell(col, row, GRID_CELL_BLOCK);
                    blockCount--;
                }
            }
        }
    }
}

void Block_update(float dt)
{
    Animation_update(defaultAnimation, dt);
}

void Block_render()
{
    for (size_t col = 0; col < GRID_COLS; col++)
    {
        for (size_t row = 0; row < GRID_ROWS; row++)
        {
            if (Grid_hasBlock(col, row))
            {
                int x = Grid_gridToCoord(col);
                int y = Grid_gridToCoord(row);
                Animation_render(defaultAnimation, x - camera.bgPositionX, y - camera.bgPositionY);
            }
        }
    }
}

static void removeAnimation(Animation *animation)
{
    World_removeGameObject(world, animation->gameObject);
}

void Block_remove(float x, float y)
{
    int col = Grid_coordToGrid(x);
    int row = Grid_coordToGrid(y);
    if (Grid_hasBlock(col, row))
    {
        Grid_setCell(col, row, GRID_CELL_NONE);
        Animation *animation = createAnimation();
        animation->gameObject->x = Grid_gridToCoord(col);
        animation->gameObject->y = Grid_gridToCoord(row);
        animation->active = true;
        animation->defaultStep = -1;
        animation->onFinish = removeAnimation;
        World_addGameObject(world, animation->gameObject);
    }
}
