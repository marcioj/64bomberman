#ifndef CONSTANTS_H
#define CONSTANTS_H

// #define HIGH_RES

#ifdef HIGH_RES
// The screen size
#define SCREEN_HT 480
#define SCREEN_WD 640
#else
#define SCREEN_HT 240
#define SCREEN_WD 320
#endif

#define SCREEN_TOP_OFFSET 30
#define GAME_OBJECT_MAX_COUNT 256
#define TILE_SIZE 16

// #define DEBUG_MEMORY
// #define DEBUG_TIME

#endif
