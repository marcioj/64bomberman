#include "texture.h"

#ifdef _ULTRA64
#include <string.h>
#include "graphic.h"

void Texture_render(Texture *texture, int x, int y)
{
    uObjBg *objBg = spritePtr++;
    memcpy(objBg, texture->data, sizeof(uObjBg));
    // objBg->s.imageW = texture->width << 2;
    // objBg->s.imageH = texture->height << 2;
    objBg->s.imageX = texture->x << 5;
    objBg->s.imageY = texture->y << 5;
    objBg->s.frameW = texture->screenWidth << 2;
    objBg->s.frameH = texture->screenHeight << 2;
    objBg->s.frameX = x << 2;
    objBg->s.frameY = y << 2;
    objBg->s.scaleW = (1 << 10);
    objBg->s.scaleH = (1 << 10);

    // prevent image underflow
    if (objBg->s.frameX < -objBg->s.frameW || objBg->s.frameY < -objBg->s.frameH)
        return;

    gDPSetRenderMode(displayListPtr++, G_RM_XLU_SPRITE, G_RM_XLU_SPRITE2);
    gDPSetCycleType(displayListPtr++, G_CYC_1CYCLE);
    gSPBgRect1Cyc(displayListPtr++, objBg);
    gDPPipeSync(displayListPtr++);

    osWritebackDCache(objBg, sizeof(uObjBg));
}
#else

extern SDL_Renderer *renderer;

SDL_Texture *Texture_load(const char *fileName)
{
    SDL_Surface *surface = IMG_Load(fileName);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

void Texture_render(Texture *texture, int x, int y)
{
    SDL_Rect srcRect;
    srcRect.x = texture->x;
    srcRect.y = texture->y;
    srcRect.w = texture->width;
    srcRect.h = texture->height;

    SDL_Rect destRect;
    destRect.x = x;
    destRect.y = y;
    destRect.w = texture->width;
    destRect.h = texture->height;

    SDL_RenderCopyEx(renderer, (SDL_Texture *)texture->data, &srcRect, &destRect, 0.0, NULL, 0);
}

#endif
