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
    // player->gameObject->x = TILE_SIZE;
    // player->gameObject->y = TILE_SIZE;
    // player->gameObject->currentAnimationIndex = 1;
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

void Player_handleCollisionEnter(GameObject *self, GameObject *collided)
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

void Player_handleCollisionExit(GameObject *self, GameObject *collided)
{
    if (collided->type == GAME_OBJECT_BOMB)
    {
        Bomb *bomb = (Bomb *)collided->data;
        // when we first create the bomb it has no collision and we can move over it
        // but if we leave the bomb area and try to enter it again it shouldn't allow
        bomb->hasCollision = true;
    }
}

void Player_render(GameObject *gameObject)
{
    Animation *currentAnimation = &gameObject->animations[gameObject->currentAnimationIndex];
    Animation_render(currentAnimation, camera.targetScreenX, camera.targetScreenY);
}

static void decreaseActiveBombs(Bomb *bomb)
{
    bomb->player->activeBombs--;
}

void Player_update(GameObject *gameObject, float dt)
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

    int animationsSize = 5;
    Animation *animations = malloc(sizeof(Animation) * animationsSize);
    for (size_t i = 0; i < animationsSize; i++)
    {
        Animation_init(animations + i);
        animations[i].texture = spritesheet;
        animations[i].speed = 0.1f;
    }

    // move left
    animations[0].spriteCol = 0;
    animations[0].spriteRow = 0,
    animations[0].steps = 3;
    animations[0].defaultStep = 1;
    // move down
    animations[1].spriteCol = 3;
    animations[1].spriteRow = 0;
    animations[1].steps = 3;
    animations[1].defaultStep = 1;
    // move right
    animations[2].spriteCol = 0;
    animations[2].spriteRow = 1;
    animations[2].steps = 3;
    animations[2].defaultStep = 1;
    // move up
    animations[3].spriteCol = 3;
    animations[3].spriteRow = 1;
    animations[3].steps = 3;
    animations[3].defaultStep = 1;
    // death
    animations[4].spriteCol = 0;
    animations[4].spriteRow = 2;
    animations[4].steps = 7;
    animations[4].speed = 0.3f;
    animations[4].defaultStep = -1;
    animations[4].automatic = true;
    animations[4].iterationCount = 1;

    obj->type = GAME_OBJECT_PLAYER;
    obj->zIndex = 1;
    obj->x = TILE_SIZE;
    obj->y = TILE_SIZE;
    obj->currentAnimationIndex = 1;
    obj->animations = animations;
    obj->render = Player_render;
    obj->update = Player_update;
    obj->onCollisionEnter = Player_handleCollisionEnter;
    obj->onCollisionExit = Player_handleCollisionExit;
    return player;
}
