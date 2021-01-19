#include "controller.h"
#include "math_utils.h"
#include "screen_printf.h"
#include "constants.h"

#ifdef _ULTRA64
#include <nusys.h>

NUContData n64ContData[1];
#else
#include <SDL2/SDL.h>

SDL_Event sdlEvent;
#endif

Controller controller[1];

int controllerDeadzone = 30;

void Controller_deadzoneCap(Controller *cont)
{
    if (fabs(cont->stick_x) <= controllerDeadzone)
        cont->stick_x = 0;
    if (fabs(cont->stick_y) <= controllerDeadzone)
        cont->stick_y = 0;
}

#ifdef _ULTRA64
void Controller_update()
{
    nuContDataGetEx(n64ContData, 0);
    controller[0].button = n64ContData->button;
    controller[0].trigger = n64ContData->trigger;
    controller[0].stick_x = n64ContData->stick_x;
    controller[0].stick_y = n64ContData->stick_y;
    Controller_deadzoneCap(&controller[0]);

#ifdef DEBUG_CONTROLLER
    Controller_print(&controller[0]);
#endif
}

#else

unsigned short Controller_getKeyForSDLKey(SDL_Keycode key)
{
    switch (key)
    {
    case SDLK_c:
        return CONT_BUTTON_A;
    case SDLK_x:
        return CONT_BUTTON_B;
    case SDLK_z:
        return CONT_BUTTON_Z;
    case SDLK_RETURN:
        return CONT_BUTTON_START;
    case SDLK_w:
        return CONT_PAD_UP;
    case SDLK_s:
        return CONT_PAD_DOWN;
    case SDLK_a:
        return CONT_PAD_LEFT;
    case SDLK_d:
        return CONT_PAD_RIGHT;
    case SDLK_q:
        return CONT_BUTTON_L;
    case SDLK_e:
        return CONT_BUTTON_R;
    case SDLK_i:
        return CONT_C_UP;
    case SDLK_k:
        return CONT_C_DOWN;
    case SDLK_j:
        return CONT_C_LEFT;
    case SDLK_l:
        return CONT_C_RIGHT;
    default:
        return 0;
    }
}

void Controller_update()
{
    Controller *cont = &controller[0];
    memset(cont, 0, sizeof(Controller));
    // SDL_PollEvent(&sdlEvent);
    // SDL_PumpEvents();
    // const Uint8 *state = SDL_GetKeyboardState(NULL);
    // if (sdlEvent.type == SDL_KEYDOWN)
    // {
    //     SDL_Keycode key = sdlEvent.key.keysym.sym;
    //     if (key == SDLK_UP)
    //     {
    //         cont->stick_y = 1;
    //     }
    //     else if (key == SDLK_DOWN)
    //     {
    //         cont->stick_y = -1;
    //     }

    //     if (key == SDLK_LEFT)
    //     {
    //         cont->stick_x = -1;
    //     }
    //     else if (key == SDLK_RIGHT)
    //     {
    //         cont->stick_x = 1;
    //     }
    //     else
    //     {
    //         cont->button = Controller_getKeyForSDLKey(key);
    //         if (!sdlEvent.key.repeat)
    //             cont->trigger = cont->button;
    //     }
    // }

    if (sdlEvent.type == SDL_KEYDOWN)
    {
        SDL_Keycode key = sdlEvent.key.keysym.sym;
        if (key == SDLK_UP)
        {
            cont->stick_y = 1;
        }
        else if (key == SDLK_DOWN)
        {
            cont->stick_y = -1;
        }

        if (key == SDLK_LEFT)
        {
            cont->stick_x = -1;
        }
        else if (key == SDLK_RIGHT)
        {
            cont->stick_x = 1;
        }
        else
        {
            cont->button = Controller_getKeyForSDLKey(key);
            if (!sdlEvent.key.repeat)
                cont->trigger = cont->button;
        }
    }
}
#endif

void Controller_print(Controller *cont)
{
    screen_printf("\ncontroller: x: %i y: %i deadzone: %i", cont->stick_x, cont->stick_y, controllerDeadzone);
}
