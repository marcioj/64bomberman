#include <stdlib.h>
#include <malloc.h>
#include "player.h"
#include "game_object.h"
#include "level.h"
#include "constants.h"
#include "camera.h"
#include "screen_printf.h"
#include "math_utils.h"
#include "bomb.h"
#include "world.h"
#include "enemy.h"
#include "textures.h"
#include "controller.h"
#include "grid.h"
#include "collision.h"
#include "block.h"
#include "world.h"

static float movingSpeed = 70;

static void restartStage(Animation *animation)
{
    Grid_clear();
    World_clear(world);
    Block_generateRandom();
    Enemy_generateRandom();
    player = Player_new();
    player->gameObject->x = TILE_SIZE;
    player->gameObject->y = TILE_SIZE;
    player->gameObject->currentAnimationIndex = 1;
    World_addGameObject(world, player->gameObject);
}

static void killPlayer(Player *player)
{
    if (player->state == PLAYER_DYING)
        return;
    player->state = PLAYER_DYING;
    Animation *animation = GameObject_playAnimation(player->gameObject, 4);
    animation->onFinish = restartStage;
}

static void Player_handleCollisionEnter(GameObject *self, GameObject *collided)
{
    Player *player = (Player *)self->data;
    if (collided->type == GAME_OBJECT_BOMB)
    {
        Bomb *bomb = (Bomb *)collided->data;
        if (bomb->state == BOMB_STATE_EXPLODING)
        {
            killPlayer(player);
        }
        else
        {
            if (bomb->hasCollision)
            {
                self->x = self->prevX;
                self->y = self->prevY;
            }
        }
    }
    else if (collided->type == GAME_OBJECT_ENEMY)
    {
        Enemy *enemy = (Enemy *)collided->data;
        if (enemy->isAlive)
            killPlayer(player);
    }
}

static void Player_handleCollisionExit(GameObject *self, GameObject *collided)
{
    if (collided->type == GAME_OBJECT_BOMB)
    {
        Bomb *bomb = (Bomb *)collided->data;
        // when we first create the bomb it has no collision and we can move over it
        // but if we leave the bomb area and try to enter it again it shouldn't allow
        bomb->hasCollision = true;
    }
}

static void Player_render(GameObject *gameObject)
{
    Animation *currentAnimation = &gameObject->animations[gameObject->currentAnimationIndex];
    Animation_render(currentAnimation, camera.targetScreenX, camera.targetScreenY + SCREEN_TOP_OFFSET);
}

static void decreaseActiveBombs(Bomb *bomb)
{
    bomb->player->activeBombs--;
}

static void Player_update(GameObject *gameObject, float dt)
{
    Controller_update();
    Player *player = (Player *)gameObject->data;
    // cannot move while player is in dying animation
    if (player->state == PLAYER_DYING)
        return;
    Controller *cont = &controller[0];

    gameObject->prevX = gameObject->x;
    gameObject->prevY = gameObject->y;

    if (cont->stick_x > 0 || cont->button & CONT_PAD_RIGHT)
    {
        GameObject_playAnimation(gameObject, 2);
        gameObject->x += movingSpeed * dt;
    }
    else if (cont->stick_x < 0 || cont->button & CONT_PAD_LEFT)
    {
        GameObject_playAnimation(gameObject, 0);
        gameObject->x -= movingSpeed * dt;
    }
    if (cont->stick_y > 0 || cont->button & CONT_PAD_UP)
    {
        GameObject_playAnimation(gameObject, 3);
        gameObject->y -= movingSpeed * dt;
    }
    else if (cont->stick_y < 0 || cont->button & CONT_PAD_DOWN)
    {
        GameObject_playAnimation(gameObject, 1);
        gameObject->y += movingSpeed * dt;
    }
    // put bomb
    if (cont->trigger & CONT_BUTTON_A)
    {
        float bombX = roundf(gameObject->x / TILE_SIZE) * TILE_SIZE;
        float bombY = roundf(gameObject->y / TILE_SIZE) * TILE_SIZE;
        int gridCol = Grid_coordToGrid(bombX);
        int gridRow = Grid_coordToGrid(bombY);
        if (player->activeBombs < player->maxActiveBombs && Grid_getCell(gridCol, gridRow) == 0)
        {
            Bomb *bomb = Bomb_new();
            bomb->player = player;
            bomb->explosionRange = player->explosionRange;
            GameObject *obj = bomb->gameObject;
            bomb->onExplosionEnd = decreaseActiveBombs;
            World_addGameObject(world, obj);
            obj->x = bombX;
            obj->y = bombY;
            Grid_setCell(Grid_coordToGrid(bombX), Grid_coordToGrid(bombY), GRID_CELL_BOMB);
            GameObject_playAnimation(obj, 0);
            player->activeBombs++;
        }
    }
    handleCollision(gameObject);
    camera.targetX = gameObject->x;
    camera.targetY = gameObject->y;
    Camera_update(&camera);
}

Player *Player_new()
{
    Player *player = malloc(sizeof(Player));
    GameObject *obj = GameObject_new();
    player->state = PLAYER_ALIVE;
    player->gameObject = obj;
    player->explosionRange = 1;
    player->activeBombs = 0;
    player->maxActiveBombs = 3;
    obj->data = player;

    int animationsCount = 5;
    Animation *animations = malloc(sizeof(Animation) * animationsCount);
    AnimationStep *steps = NULL;
    Tile *tiles = NULL;
    for (size_t i = 0; i < animationsCount; i++)
    {
        Animation_init(animations + i);
        animations[i].iterationCount = -1;
        animations[i].automatic = false;
    }

    // move left
    steps = malloc(sizeof(AnimationStep) * 3);
    tiles = malloc(sizeof(Tile) * 3);
    for (size_t i = 0; i < 3; i++)
    {
        tiles[i].row = 0;
        tiles[i].col = i;
        tiles[i].sheet = spritesheet;
        steps[i].timeMs = 0.1f;
        steps[i].tile = &tiles[i];
        steps[i].texture = NULL;
    }
    animations[0].steps = steps;
    animations[0].stepsCount = 3;
    animations[0].defaultStep = 1;
    // move down
    steps = malloc(sizeof(AnimationStep) * 3);
    tiles = malloc(sizeof(Tile) * 3);
    for (size_t i = 0; i < 3; i++)
    {
        tiles[i].row = 0;
        tiles[i].col = i + 3;
        tiles[i].sheet = spritesheet;
        steps[i].timeMs = 0.1f;
        steps[i].tile = &tiles[i];
        steps[i].texture = NULL;
    }
    animations[1].steps = steps;
    animations[1].stepsCount = 3;
    animations[1].defaultStep = 1;
    // move right
    steps = malloc(sizeof(AnimationStep) * 3);
    tiles = malloc(sizeof(Tile) * 3);
    for (size_t i = 0; i < 3; i++)
    {
        tiles[i].row = 1;
        tiles[i].col = i;
        tiles[i].sheet = spritesheet;
        steps[i].timeMs = 0.1f;
        steps[i].tile = &tiles[i];
        steps[i].texture = NULL;
    }
    animations[2].steps = steps;
    animations[2].stepsCount = 3;
    animations[2].defaultStep = 1;
    // move up
    steps = malloc(sizeof(AnimationStep) * 3);
    tiles = malloc(sizeof(Tile) * 3);
    for (size_t i = 0; i < 3; i++)
    {
        tiles[i].row = 1;
        tiles[i].col = i + 3;
        tiles[i].sheet = spritesheet;
        steps[i].timeMs = 0.1f;
        steps[i].tile = &tiles[i];
        steps[i].texture = NULL;
    }
    animations[3].steps = steps;
    animations[3].stepsCount = 3;
    animations[3].defaultStep = 1;
    // death
    steps = malloc(sizeof(AnimationStep) * 7);
    tiles = malloc(sizeof(Tile) * 7);
    for (size_t i = 0; i < 7; i++)
    {
        tiles[i].row = 2;
        tiles[i].col = i;
        tiles[i].sheet = spritesheet;
        steps[i].timeMs = 0.3f;
        steps[i].tile = &tiles[i];
        steps[i].texture = NULL;
    }
    animations[4].steps = steps;
    animations[4].stepsCount = 7;
    animations[4].defaultStep = -1;
    animations[4].iterationCount = 1;
    animations[4].automatic = true;

    obj->type = GAME_OBJECT_PLAYER;
    obj->zIndex = 1;
    obj->x = TILE_SIZE;
    obj->y = TILE_SIZE;
    obj->currentAnimationIndex = 1;
    obj->animations = animations;
    obj->animationsCount = animationsCount;
    obj->render = Player_render;
    obj->update = Player_update;
    obj->onCollisionEnter = Player_handleCollisionEnter;
    obj->onCollisionExit = Player_handleCollisionExit;
    return player;
}
