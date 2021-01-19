#include <nusys.h>
#include "graphic.h"

/*
  The viewport structure 
  The conversion from (-1,-1,-1)-(1,1,1).  The decimal part is 2-bit.
 */
static Vp vp = {
    SCREEN_WD * 2, SCREEN_HT * 2, G_MAXZ / 2, 0, /* The scale factor  */
    SCREEN_WD * 2, SCREEN_HT * 2, G_MAXZ / 2, 0, /* Move  */
};

/*
  The initialization of RDP 
*/
Gfx setup_rdpstate[] = {
    gsDPPipeSync(),
    gsDPPipelineMode(G_PM_1PRIMITIVE),
    gsDPSetTextureLOD(G_TL_TILE),
    gsDPSetTextureLUT(G_TT_NONE),
    gsDPSetTextureDetail(G_TD_CLAMP),
    gsDPSetTexturePersp(G_TP_NONE),
    gsDPSetTextureFilter(G_TF_BILERP),
    gsDPSetTextureConvert(G_TC_FILT),
    gsDPSetCombineMode(G_CC_DECALRGBA, G_CC_DECALRGBA),
    gsDPSetCombineKey(G_CK_NONE),
    gsDPSetAlphaCompare(G_AC_NONE),
    gsDPSetColorDither(G_CD_DISABLE),
    gsDPSetScissor(G_SC_NON_INTERLACE, 0, 0, SCREEN_WD, SCREEN_HT),
    gsSPEndDisplayList(),
};

/*
  The initialization of RSP 
*/
Gfx setup_rspstate[] = {
    gsSPEndDisplayList(),
};
