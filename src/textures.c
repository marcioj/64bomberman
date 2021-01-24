#include "textures.h"
#include "constants.h"

SpriteSheet *spritesheet;
Texture *levelBg;

#ifdef _ULTRA64
#include <ultra64.h>

extern uObjBg sp_bomberman_bg;
extern uObjBg sp_field_bg;
#endif
#include <malloc.h>

void Textures_init()
{
#ifdef _ULTRA64
    guS2DInitBg(&sp_field_bg);
    void *bombdata = &sp_bomberman_bg;
    void *leveldata = &sp_field_bg;
#else
    void *bombdata = Texture_load("assets/images/bomberman.png");
    void *leveldata = Texture_load("assets/images/field.png");
#endif

    Texture *bombtex = malloc(sizeof(Texture));
    bombtex->data = bombdata;
    bombtex->width = 224;
    bombtex->height = 383;
    bombtex->x = 0;
    bombtex->y = 0;
    spritesheet = malloc(sizeof(SpriteSheet));
    spritesheet->texture = bombtex;
    spritesheet->tileHeight = 16;
    spritesheet->tileWidth = 16;
    spritesheet->tileCols = 29;
    spritesheet->tileRows = 11;

    levelBg = malloc(sizeof(Texture));
    levelBg->data = leveldata;
    levelBg->width = 496;
    levelBg->height = 225;
    levelBg->screenWidth = SCREEN_WD;
    levelBg->screenHeight = SCREEN_HT;
    levelBg->x = 0;
    levelBg->y = 0;
}
