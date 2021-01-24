#include "level.h"
#include "constants.h"
#include "math_utils.h"
#include "screen_printf.h"
#include "textures.h"

int Level_getHeight()
{
    return levelBg->height;
}

int Level_getWidth()
{
    return levelBg->width;
}

#ifdef _ULTRA64
void Level_setBackgroundPosition(int x, int y)
{
    levelBg->x = x;
    levelBg->y = y;
}

void Level_render()
{
    Texture_render(levelBg, 0, SCREEN_TOP_OFFSET);
}
#else
static int imageX, imageY;

void Level_setBackgroundPosition(int x, int y)
{
    imageX = -x;
    imageY = -y;
}

void Level_render()
{
    Texture_render(levelBg, imageX, imageY + SCREEN_TOP_OFFSET);
}
#endif
