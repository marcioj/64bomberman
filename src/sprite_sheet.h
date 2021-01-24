#ifndef SPRITE_SHEET_H
#define SPRITE_SHEET_H

#include "texture.h"

typedef struct SpriteSheet
{
    Texture *texture;
    int tileWidth;
    int tileHeight;
    int tileCols;
    int tileRows;
} SpriteSheet;

void SpriteSheet_render(SpriteSheet *self, int col, int row, int x, int y);

#endif
