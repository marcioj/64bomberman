#include "enemy.h"
#include "world.h"
#include "bomb.h"
#include "block.h"
#include "level.h"
#include <malloc.h>
#include "math_utils.h"
#include "textures.h"
#include "camera.h"
#include "grid.h"
#include "screen_printf.h"
#include "collision.h"

static float movingSpeed = 30;

static void removeEnemy(Animation *animation)
{
    World_removeGameObject(world, (GameObject *)animation->caller);
}

static void Enemy_render(GameObject *obj)
{
    Animation *currentAnimation = &obj->animations[obj->currentAnimationIndex];
    Animation_render(currentAnimation, obj->x - camera.bgPositionX, obj->y - camera.bgPositionY + SCREEN_TOP_OFFSET);
}

static void Enemy_handleCollisionEnter(GameObject *self, GameObject *collided)
{
    if (collided->type == GAME_OBJECT_BOMB)
    {
        Bomb *bomb = (Bomb *)collided->data;
        Enemy *enemy = (Enemy *)self->data;
        if (bomb->state == BOMB_STATE_EXPLODING && enemy->isAlive)
        {
            enemy->isAlive = false;
            GameObject_playAnimation(self, 2); // death
        }
    }
}

static void moveRight(Enemy *enemy)
{
    enemy->moveX = 1;
    enemy->moveY = 0;
}

static void moveLeft(Enemy *enemy)
{
    enemy->moveX = -1;
    enemy->moveY = 0;
}

static void moveUp(Enemy *enemy)
{
    enemy->moveX = 0;
    enemy->moveY = -1;
}

static void moveDown(Enemy *enemy)
{
    enemy->moveX = 0;
    enemy->moveY = 1;
}

static void Enemy_update(GameObject *gameObject, float dt)
{
    Enemy *enemy = (Enemy *)gameObject->data;
    if (!enemy->isAlive)
        return;

    gameObject->prevX = gameObject->x;
    gameObject->prevY = gameObject->y;
    gameObject->x += enemy->moveX * movingSpeed * dt;
    gameObject->y += enemy->moveY * movingSpeed * dt;

    float epsilon = 0.2f;
    int col = roundf(gameObject->x / TILE_SIZE);
    int row = roundf(gameObject->y / TILE_SIZE);
    float colTmp = (gameObject->x / TILE_SIZE);
    float rowTmp = (gameObject->y / TILE_SIZE);
    float col2 = colTmp - (int)colTmp;
    float row2 = rowTmp - (int)rowTmp;
    bool isIntersection = col % 2 != 0 && row % 2 != 0 && col2 < epsilon && row2 < epsilon;

    enemy->turnTimeout -= dt;

    if (handleCollision(gameObject) || (isIntersection && enemy->turnTimeout <= 0))
    {
        if (enemy->turnTimeout <= 0)
            enemy->turnTimeout = 3.0f;
        // enemy->turnTimeout = randomMinMaxf(3, 10);

        // choose a random direction to move
        int directions[4] = {0, 1, 2, 3};
        randomizeArray(directions, sizeof(int), 4);
        for (size_t i = 0; i < 4; i++)
        {
            int dir = directions[i];
            if (dir == 0 && !checkCollisionUp(gameObject->x, gameObject->y - TILE_SIZE / 2, NULL))
            {
                moveUp(enemy);
                break;
            }
            else if (dir == 1 && !checkCollisionDown(gameObject->x, gameObject->y + TILE_SIZE / 2, NULL))
            {
                moveDown(enemy);
                break;
            }
            else if (dir == 2 && !checkCollisionRight(gameObject->x + TILE_SIZE / 2, gameObject->y, NULL))
            {
                moveRight(enemy);
                break;
            }
            else if (dir == 3 && !checkCollisionLeft(gameObject->x - TILE_SIZE / 2, gameObject->y, NULL))
            {
                moveLeft(enemy);
                break;
            }
        }
    }

    if (enemy->moveX > 0 || enemy->moveY < 0)
        GameObject_playAnimation(gameObject, 0);
    else if (enemy->moveX < 0 || enemy->moveY > 0)
        GameObject_playAnimation(gameObject, 1);

    // camera.targetX = gameObject->x;
    // camera.targetY = gameObject->y;
    // Camera_update(&camera);
}

void Enemy_generateRandom()
{
    int enemyCount = 6;

    while (true)
    {
        for (size_t col = 5; col < GRID_COLS; col++)
        {
            for (size_t row = 0; row < GRID_ROWS; row++)
            {
                if (enemyCount == 0)
                    return;
                int x = Grid_gridToCoord(col);
                int y = Grid_gridToCoord(row);
                if (Grid_hasCollision(col, row))
                    continue;

                if (randomMinMax(0, 50) == 0)
                {
                    Enemy *enemy = Enemy_new();
                    enemy->gameObject->x = x;
                    enemy->gameObject->y = y;
                    World_addGameObject(world, enemy->gameObject);
                    enemyCount--;
                }
            }
        }
    }
}

Enemy *Enemy_new()
{
    Enemy *enemy = malloc(sizeof(Enemy));
    GameObject *obj = GameObject_new();
    enemy->gameObject = obj;
    enemy->isAlive = true;
    enemy->moveX = 1;
    enemy->moveY = 0;
    enemy->turnTimeout = 3.0f;
    obj->type = GAME_OBJECT_ENEMY;
    obj->zIndex = 1;
    obj->data = enemy;
    obj->update = Enemy_update;
    obj->render = Enemy_render;
    obj->onCollisionEnter = Enemy_handleCollisionEnter;

    int animationsCount = 3;
    Animation *animations = malloc(sizeof(Animation) * animationsCount);
    AnimationStep *steps = NULL;
    Tile *tiles = NULL;
    for (size_t i = 0; i < animationsCount; i++)
    {
        Animation_init(animations + i);
        animations[i].iterationCount = -1;
        animations[i].automatic = false;
    }
    // facing right
    steps = malloc(sizeof(AnimationStep) * 3);
    tiles = malloc(sizeof(Tile) * 3);
    for (size_t i = 0; i < 3; i++)
    {
        tiles[i].row = 15;
        tiles[i].col = i;
        tiles[i].sheet = spritesheet;
        steps[i].timeMs = 0.3f;
        steps[i].tile = &tiles[i];
        steps[i].texture = NULL;
    }
    animations[0].stepsCount = 3;
    animations[0].steps = steps;
    // facing left
    steps = malloc(sizeof(AnimationStep) * 3);
    tiles = malloc(sizeof(Tile) * 3);
    for (size_t i = 0; i < 3; i++)
    {
        tiles[i].row = 15;
        tiles[i].col = i + 3;
        tiles[i].sheet = spritesheet;
        steps[i].timeMs = 0.3f;
        steps[i].tile = &tiles[i];
        steps[i].texture = NULL;
    }
    animations[1].stepsCount = 3;
    animations[1].steps = steps;
    // death
    steps = malloc(sizeof(AnimationStep) * 5);
    tiles = malloc(sizeof(Tile) * 5);
    for (size_t i = 0; i < 5; i++)
    {
        tiles[i].row = 15;
        tiles[i].col = i + 6;
        tiles[i].sheet = spritesheet;
        steps[i].timeMs = 0.3f;
        steps[i].tile = &tiles[i];
        steps[i].texture = NULL;
    }
    steps[0].timeMs = 1.0f;
    animations[2].stepsCount = 5;
    animations[2].steps = steps;
    animations[2].automatic = true;
    animations[2].iterationCount = 1;
    obj->animations = animations;
    obj->animationsCount = animationsCount;
    return enemy;
}
