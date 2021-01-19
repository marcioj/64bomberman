#ifndef _GRAPHIC_H_
#define _GRAPHIC_H_

#include <ultra64.h>
#include <PR/gs2dex.h>
#include "constants.h"

#define MAX_GRAPHICS_TASKS 2
// The maximum length of the display list of one task
#define MAX_DISPLAY_LIST_COMMANDS 4096

// a struct to hold graphics data used by the RCP which can change at runtime
typedef struct GraphicsTask
{
    Gfx displayList[MAX_DISPLAY_LIST_COMMANDS];
    uObjBg sprites[4096];
} GraphicsTask;

extern struct GraphicsTask graphicsTasks[MAX_GRAPHICS_TASKS];
extern Gfx *displayListPtr;
extern uObjBg *spritePtr;

extern GraphicsTask *gfxSwitchTask();
extern void gfxRCPInit();
extern void gfxClearCfb();

extern Gfx setup_rdpstate[];
extern Gfx setup_rspstate[];

#endif /* _GRAPHIC_H_ */
