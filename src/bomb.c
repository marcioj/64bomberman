#include "bomb.h"
#include <malloc.h>
#include "constants.h"
#include "screen_printf.h"
#include "world.h"
#include "math_utils.h"
#include "level.h"
#include "block.h"
#include "grid.h"
#include "textures.h"

void removeExplosion(Animation *animation)
{
    // is center explosion animation
    if (animation == animation->gameObject->animations + 1)
    {
        Bomb *center = (Bomb *)animation->gameObject->data;
        if (center->starterBomb->onExplosionEnd)
        {
            center->starterBomb->onExplosionEnd(center->starterBomb);
            World_removeGameObject(world, center->starterBomb->gameObject);
        }
    }
    World_removeGameObject(world, animation->gameObject);
}

static bool addExplosion(Bomb *starter, float gridX, float gridY, int animationIndex)
{
    int col = Grid_coordToGrid(gridX);
    int row = Grid_coordToGrid(gridY);

    if (Grid_hasPillar(col, row))
        return false;

    if (Grid_hasBlock(col, row))
    {
        Block_remove(gridX, gridY);
        return false;
    }

    Bomb *bomb = Bomb_new();
    World_addGameObject(world, bomb->gameObject);
    bomb->state = BOMB_STATE_EXPLODING;
    bomb->starterBomb = starter;
    bomb->gameObject->x = gridX;
    bomb->gameObject->y = gridY;
    Animation *animation = GameObject_playAnimation(bomb->gameObject, animationIndex);
    animation->iterationCount = 1;
    animation->onFinish = removeExplosion;
    return true;
}

void Bomb_update(GameObject *obj, float dt)
{
    Bomb *bomb = (Bomb *)obj->data;
    if (bomb->age > 0)
        bomb->age -= dt;
    else
    {
        if (bomb->state == BOMB_STATE_EXPLODING)
            return;
        bomb->state = BOMB_STATE_EXPLODING;
        float gridX = obj->x;
        float gridY = obj->y;
        Grid_setCell(Grid_coordToGrid(obj->x), Grid_coordToGrid(obj->y), GRID_CELL_NONE);
        addExplosion(bomb, gridX, gridY, 1);
        bool upFire, downFire, leftFire, rightFire;
        upFire = downFire = leftFire = rightFire = true;

        for (size_t i = 0; i < bomb->explosionRange; i++)
        {
            int tileOffset = TILE_SIZE * (i + 1);
            bool isLast = i == bomb->explosionRange - 1;
            // vertical 3 horizontal 7
            if (upFire)
                upFire = addExplosion(bomb, gridX, gridY - tileOffset, isLast ? 4 : 7);
            if (downFire)
                downFire = addExplosion(bomb, gridX, gridY + tileOffset, isLast ? 6 : 7);
            if (leftFire)
                leftFire = addExplosion(bomb, gridX - tileOffset, gridY, isLast ? 2 : 3);
            if (rightFire)
                rightFire = addExplosion(bomb, gridX + tileOffset, gridY, isLast ? 5 : 3);
        }
    }
}

void Bomb_handleCollisionEnter(GameObject *self, GameObject *collided)
{
    if (collided->type == GAME_OBJECT_BOMB)
    {
        Bomb *bomb = (Bomb *)self->data;
        Bomb *collidedBomb = (Bomb *)collided->data;
        // collidedBomb->state is the problem
        if (collidedBomb->state == BOMB_STATE_EXPLODING && bomb->state != BOMB_STATE_EXPLODING)
        {
            // if fire hits a bomb. make it explode immediatelly
            bomb->age = 0;
        }
    }
}

Bomb *Bomb_new()
{
    Bomb *bomb = malloc(sizeof(Bomb));
    GameObject *obj = GameObject_new();
    bomb->gameObject = obj;
    bomb->hasCollision = false;
    bomb->explosionRange = 1;
    bomb->age = 2;
    bomb->state = BOMB_STATE_NEW;
    bomb->starterBomb = NULL;
    bomb->onExplosionEnd = NULL;
    obj->data = bomb;
    obj->type = GAME_OBJECT_BOMB;
    obj->update = Bomb_update;
    obj->onCollisionEnter = Bomb_handleCollisionEnter;
    int animationsSize = 8;
    Animation *animations = malloc(sizeof(Animation) * animationsSize);
    for (size_t i = 0; i < animationsSize; i++)
    {
        Animation_init(animations + i);
        animations[i].texture = spritesheet;
        animations[i].direction = ANIMATION_DIRECTION_ALTERNATE;
        animations[i].speed = 0.07f;
        animations[i].spriteRow = 3 + i;
        animations[i].steps = 4;
        animations[i].defaultStep = -1;
        animations[i].automatic = true;
    }

    // bomb
    animations[0].direction = ANIMATION_DIRECTION_NORMAL;
    animations[0].steps = 3;
    animations[0].defaultStep = 0;
    animations[0].speed = 0.3f;
    obj->animations = animations;
    return bomb;
}
