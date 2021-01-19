#ifndef PLAYER_H
#define PLAYER_H

#include "game_object.h"

typedef enum PlayerState
{
    PLAYER_ALIVE,
    PLAYER_DYING,
} PlayerState;

typedef struct Player
{
    PlayerState state;
    GameObject *gameObject;
    int activeBombs;
    int maxActiveBombs;
    int explosionRange;
} Player;

extern Player *player;

Player *Player_new();

#endif
