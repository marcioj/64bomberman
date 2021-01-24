#include <nusys.h>
#include <assert.h>
#include <stdbool.h>
#include <vec.h>
#include <debug.h>
#include "math_utils.h"
#include "graphic.h"
#include "audio.h"
#include "main.h"
#include "stage00.h"
#include "screen_printf.h"
#include "clock.h"
#include "game_object.h"
#include "player.h"
#include "block.h"
#include "level.h"
#include "sp_bomberman.h"
#include "sp_field_bg.h"
#include "constants.h"
#include "camera.h"
#include "bomb.h"
#include "world.h"
#include "enemy.h"
#include "textures.h"

World *world;
Player *player;
Camera camera;

void initStage00()
{
    srand(osGetTime());
    Textures_init();
    Clock_nextTick();
    world = World_new();
    player = Player_new();
    World_addGameObject(world, player->gameObject);
    Block_init();
    Block_generateRandom();
    Enemy_generateRandom();
    Audio_play(0);
}

void updateGame00(float dt)
{
    World_update(world, dt);
    Block_update(dt);
#ifdef CONSOLE
    screen_printf("console build\n");
#else
    screen_printf("emulator build\n");
#endif
}

void makeDL00()
{
    GraphicsTask *gfxTask = gfxSwitchTask();
    gfxRCPInit();
    gfxClearCfb();

    Level_render();
    Block_render();
    World_render(world);

    gDPFullSync(displayListPtr++);
    gSPEndDisplayList(displayListPtr++);

    assert(displayListPtr - gfxTask->displayList < MAX_DISPLAY_LIST_COMMANDS);

    osWritebackDCache(&gfxTask, sizeof(gfxTask));

    nuGfxTaskStart(
        gfxTask->displayList,
        (int)(displayListPtr - gfxTask->displayList) * sizeof(Gfx),
        NU_GFX_UCODE_S2DEX2,
        NU_SC_NOSWAPBUFFER);
#ifdef DEBUG_TIME
    screen_printf("FPS: %i\ndt: %f\n", (int)(1.0f / Clock_deltaTime()), Clock_deltaTime());
#endif
#ifdef DEBUG_MEMORY
    struct malloc_status_st m_status;
    malloc_memcheck(&m_status);
    screen_printf("memory: free: %i used: %i\n", m_status.freeMemSize, m_status.useMemSize);
#endif
    nuDebConDisp(NU_SC_SWAPBUFFER);
    nuDebConClear(0);
}

// the nusystem callback for the stage, called once per frame
void stage00(int pendingGfx)
{
    nuDebConTextColor(0, NU_DEB_CON_TEXT_BLACK);

    Clock_nextTick();
    if (pendingGfx < 2)
        makeDL00();

    updateGame00(Clock_deltaTime());
}
