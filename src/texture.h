#ifndef TEXTURE_H
#define TEXTURE_H

#ifdef _ULTRA64
#include <ultra64.h>
#include <PR/gs2dex.h>

// #define TEXTURE uObjBg

#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// #define TEXTURE SDL_Texture

SDL_Texture *Texture_load(const char *fileName);

#endif

typedef struct Texture
{
    void *data;
    int x;
    int y;
    int width;
    int height;
    int screenWidth;
    int screenHeight;
} Texture;

void Texture_render(Texture *texture, int x, int y);

#endif
