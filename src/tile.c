#include "tile.h"

void Tile_render(Tile *self, int x, int y)
{
    SpriteSheet_render(self->sheet, self->col, self->row, x, y);
}
