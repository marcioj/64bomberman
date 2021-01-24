#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <stdbool.h>
#include "constants.h"
#include "clock.h"
#include "world.h"
#include "level.h"
#include "player.h"
#include "enemy.h"
#include "block.h"
#include "camera.h"
#include <vec.h>
#include "texture.h"
#include "textures.h"
#include "sprite_sheet.h"
#include "tile.h"
#include "animation.h"

bool isRunning;
World *world;
Player *player;
Camera camera;
SDL_Window *window;
SDL_Renderer *renderer;
extern SDL_Event sdlEvent;

int initializeSDL()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        printf("Error initializing SDL: %s\n", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow("GAME",
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              SCREEN_WD * 3, SCREEN_HT * 3, 0);
    if (!window)
    {
        printf("Error creating SDL window: %s\n", SDL_GetError());
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer)
    {
        printf("Error creating SDL renderer: %s\n", SDL_GetError());
        return 1;
    }
    return 0;
}

void processInput()
{
    SDL_PollEvent(&sdlEvent);
    switch (sdlEvent.type)
    {
    case SDL_QUIT:
    {
        isRunning = false;
        break;
    }
    case SDL_KEYDOWN:
    {
        if (sdlEvent.key.keysym.sym == SDLK_ESCAPE)
        {
            isRunning = false;
        }
    }
    default:
    {
        break;
    }
    }
}

int main(int argc, char const *argv[])
{
    int sdlStatus;
    if ((sdlStatus = initializeSDL()) != 0)
    {
        return sdlStatus;
    }

    SDL_RenderSetLogicalSize(renderer, SCREEN_WD, SCREEN_HT);

    isRunning = true;
    Clock_nextTick();

    srand(Clock_timeMs());
    Textures_init();
    world = World_new();
    player = Player_new();
    World_addGameObject(world, player->gameObject);
    Block_init();
    Block_generateRandom();
    Enemy_generateRandom();

    while (isRunning)
    {
        Clock_nextTick();
        float dt = Clock_deltaTime();
        if (dt < 0.016f)
        {
            float waitTime = 0.016f - dt;
            SDL_Delay(waitTime);
        }

        processInput();
        World_update(world, dt);
        Block_update(dt);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        Level_render();
        Block_render();
        World_render(world);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
