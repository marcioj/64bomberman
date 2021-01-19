#include "textures.h"

TEXTURE *spritesheet;
TEXTURE *levelBg;

#ifdef _ULTRA64
extern uObjBg sp_bomberman_bg;
extern uObjBg sp_field_bg;

void Textures_init()
{
    spritesheet = &sp_bomberman_bg;
    // levelBg = &sp_field_bg;
}
#else
void Textures_init()
{
    spritesheet = Texture_load("assets/images/bomberman.png");
    // levelBg = Texture_load("assets/images/field.png");
}
#endif
