#ifndef ENEMY_H
#define ENEMY_H

#include "game_object.h"

typedef struct Enemy
{
    bool isAlive;
    float turnTimeout;
    GameObject *gameObject;
    float moveX;
    float moveY;
} Enemy;

Enemy *Enemy_new();

void Enemy_generateRandom();

#endif
