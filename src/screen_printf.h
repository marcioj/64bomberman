#ifndef SCREEN_PRINTF_H
#define SCREEN_PRINTF_H

#include <stdio.h>

#ifdef _ULTRA64

#include <nusys.h>

#define screen_printf(text, ...)                             \
    do                                                       \
    {                                                        \
        char _print_screen_buffer_[256];                     \
        sprintf(_print_screen_buffer_, text, ##__VA_ARGS__); \
        nuDebConCPuts(0, _print_screen_buffer_);             \
    } while (0)
#else

#define screen_printf(text, ...) printf(text, ##__VA_ARGS__)

#endif

#endif
