#ifndef TILE_H
#define TILE_H

#include "sprite_sheet.h"

typedef struct Tile
{
    SpriteSheet *sheet;
    int row;
    int col;
} Tile;

void Tile_render(Tile *self, int x, int y);

#endif
