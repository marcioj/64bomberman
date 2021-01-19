#include <nusys.h>
#include <malloc.h>
#include <debug.h>
#include "audio.h"
#include "stage00.h"
#include "graphic.h"
#include "mem_heap.h"

extern char mem_heap[MEM_HEAP_SIZE];

#define ZBUFFER_ADDR (0x80400000 - AUDIO_HEAP_SIZE - 640 * 480 * 2 * 3)
#define CFB_HIGH_ADDR0 (0x80400000 - AUDIO_HEAP_SIZE - 640 * 480 * 2 * 2)
#define CFB_HIGH_ADDR1 (0x80400000 - AUDIO_HEAP_SIZE - 640 * 480 * 2 * 1)

#define CFB_LOW_ADDR0 (0x80400000 - AUDIO_HEAP_SIZE - 320 * 240 * 2 * 3)
#define CFB_LOW_ADDR1 (0x80400000 - AUDIO_HEAP_SIZE - 320 * 240 * 2 * 2)
#define CFB_LOW_ADDR2 (0x80400000 - AUDIO_HEAP_SIZE - 320 * 240 * 2 * 1)

/* High resolution frame buffer address array. */
u16 *HighFrameBuf[2] = {
    (u16 *)CFB_HIGH_ADDR0,
    (u16 *)CFB_HIGH_ADDR1,
};

void mainproc(void)
{
    nuGfxInit();
    // this can cause issues on emulators so is only enabled for console builds
#ifdef CONSOLE
    debug_initialize();
#endif

    if (InitHeap(mem_heap, sizeof(mem_heap)) == -1)
    {
        // assert("Cannot allocate memory");
    }

    nuContInit();
    Audio_init();

    nuGfxTaskAllEndWait();
#ifdef HIGH_RES
    osViSetMode(&osViModeTable[OS_VI_NTSC_HAN1]);
    osViSetSpecialFeatures(OS_VI_GAMMA_DITHER_ON);
    nuGfxDisplayOff();
    nuGfxSetCfb(HighFrameBuf, 2);
    nuGfxSetZBuffer((u16 *)(ZBUFFER_ADDR));
#endif

    initStage00();
    nuGfxFuncSet((NUGfxFunc)stage00);
    nuGfxDisplayOn();

    while (1)
        ;
}
