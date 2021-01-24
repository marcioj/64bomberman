#include "sprite_sheet.h"

void SpriteSheet_render(SpriteSheet *self, int col, int row, int x, int y)
{
    self->texture->x = self->tileWidth * col;
    self->texture->y = self->tileHeight * row;
    self->texture->width = self->tileWidth;
    self->texture->height = self->tileHeight;
    self->texture->screenWidth = self->tileWidth;
    self->texture->screenHeight = self->tileHeight;
    Texture_render(self->texture, x, y);
}
