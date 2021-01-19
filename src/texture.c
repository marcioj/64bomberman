#include "texture.h"

#ifdef _ULTRA64
#else

extern SDL_Renderer *renderer;

SDL_Texture *Texture_load(const char *fileName)
{
    SDL_Surface *surface = IMG_Load(fileName);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

void Texture_render(SDL_Texture *texture, SDL_Rect sourceRectangle, SDL_Rect destinationRectangle, SDL_RendererFlip flip)
{
    SDL_RenderCopyEx(renderer, texture, &sourceRectangle, &destinationRectangle, 0.0, NULL, flip);
}
#endif
