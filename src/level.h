#ifndef LEVEL_H
#define LEVEL_H

#include <stdbool.h>
#include "game_object.h"

int Level_getHeight();

int Level_getWidth();

void Level_setBackgroundPosition(int x, int y);

void Level_render();

#endif
