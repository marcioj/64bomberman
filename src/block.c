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

static void renderAnimation(GameObject *obj)
{
    Animation *currentAnimation = &obj->animations[obj->currentAnimationIndex];
    Animation_render(currentAnimation, obj->x - camera.bgPositionX, obj->y - camera.bgPositionY + SCREEN_TOP_OFFSET);
}

Animation *createAnimation()
{
    GameObject *gameObject = GameObject_new();
    gameObject->render = renderAnimation;
    gameObject->zIndex = 2;
    Animation *animation = Animation_new();
    AnimationStep *steps = malloc(sizeof(AnimationStep) * 7);
    Tile *tiles = malloc(sizeof(Tile) * 7);
    for (size_t i = 0; i < 7; i++)
    {
        tiles[i].row = 3;
        tiles[i].col = i + 4;
        tiles[i].sheet = spritesheet;
        steps[i].timeMs = 0.07f;
        steps[i].tile = &tiles[i];
        steps[i].texture = NULL;
    }
    animation->steps = steps;
    animation->stepsCount = 7;
    animation->defaultStep = 0;
    animation->iterationCount = 1;
    animation->automatic = true;
    gameObject->animations = animation;
    gameObject->animationsCount = 1;
    animation->caller = gameObject;
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
                Animation_render(defaultAnimation, x - camera.bgPositionX, y - camera.bgPositionY + SCREEN_TOP_OFFSET);
            }
        }
    }
}

static void removeGameObject(Animation *animation)
{
    World_removeGameObject(world, (GameObject *)animation->caller);
}

void Block_remove(float x, float y)
{
    int col = Grid_coordToGrid(x);
    int row = Grid_coordToGrid(y);
    if (Grid_hasBlock(col, row))
    {
        Grid_setCell(col, row, GRID_CELL_NONE);
        Animation *animation = createAnimation();
        ((GameObject *)animation->caller)->x = Grid_gridToCoord(col);
        ((GameObject *)animation->caller)->y = Grid_gridToCoord(row);
        Animation_play(animation);
        animation->onFinish = removeGameObject;
        World_addGameObject(world, (GameObject *)animation->caller);
    }
}
