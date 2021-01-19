#ifndef TEXTURE_H
#define TEXTURE_H

#ifdef _ULTRA64
#include <ultra64.h>
#include <PR/gs2dex.h>

#define TEXTURE uObjBg

#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define TEXTURE SDL_Texture

SDL_Texture *Texture_load(const char *fileName);

void Texture_render(SDL_Texture *texture, SDL_Rect sourceRectangle, SDL_Rect destinationRectangle, SDL_RendererFlip flip);

#endif

#endif
