#include "level.h"
#include "constants.h"
#include "math_utils.h"
#include "screen_printf.h"
#include "textures.h"
#include "grid.h"

#ifdef _ULTRA64
#include "graphic.h"

extern uObjBg sp_field_bg;

void Level_init()
{
    guS2DInitBg(&sp_field_bg);
    sp_field_bg.b.frameH = SCREEN_HT << 2;
    sp_field_bg.b.frameW = SCREEN_WD << 2;
    sp_field_bg.s.scaleW = (1 << 10);
    sp_field_bg.s.scaleH = (1 << 10);
    sp_field_bg.s.imageYorig = sp_field_bg.s.imageY;
    sp_field_bg.s.imageH -= (16 << 2); // remove some unwanted sprites in the bottom
    sp_field_bg.b.frameY = SCREEN_TOP_OFFSET << 2;
}

void Level_render()
{
    gDPSetRenderMode(displayListPtr++, G_RM_XLU_SPRITE, G_RM_XLU_SPRITE2);
    gDPSetCycleType(displayListPtr++, G_CYC_1CYCLE);
    gDPSetTextureFilter(displayListPtr++, G_TF_POINT);
    gSPBgRect1Cyc(displayListPtr++, &sp_field_bg);
    osWritebackDCache(&sp_field_bg, sizeof(sp_field_bg));
    gDPPipeSync(displayListPtr++);
}

int Level_getHeight()
{
    return sp_field_bg.s.imageH >> 2;
}

int Level_getWidth()
{
    return sp_field_bg.s.imageW >> 2;
}

void Level_setBackgroundPosition(int x, int y)
{
    sp_field_bg.s.imageX = x << 5;
    sp_field_bg.s.imageY = y << 5;
}
#else
float imageX, imageY;
SDL_Texture *sp_field_bg;

int Level_getHeight()
{
    return 210;
}

int Level_getWidth()
{
    return 496;
}

void Level_init()
{
    sp_field_bg = Texture_load("assets/images/field.png");
}

void Level_render()
{
    SDL_Rect source = (SDL_Rect){
        .h = Level_getHeight(),
        .w = Level_getWidth() - imageX,
        .x = imageX,
        .y = imageY,
    };
    SDL_Rect dest = (SDL_Rect){
        .h = source.h,
        .w = source.w,
        .x = 0,
        .y = SCREEN_TOP_OFFSET,
    };
    Texture_render(sp_field_bg, source, dest, 0);
}

void Level_setBackgroundPosition(int x, int y)
{
    imageX = x;
    imageY = y;
}
#endif
