#ifndef BOMB_H
#define BOMB_H

#include "game_object.h"
#include "player.h"

typedef enum BombState
{
    BOMB_STATE_NEW,
    BOMB_STATE_EXPLODING,
} BombState;

typedef struct Bomb Bomb;

typedef struct Bomb
{
    BombState state;
    Bomb *starterBomb;
    Player *player;
    bool hasCollision;
    int explosionRange;
    float age;
    GameObject *gameObject;
    void (*onExplosionEnd)(Bomb *self);
} Bomb;

Bomb *Bomb_new();

#endif
