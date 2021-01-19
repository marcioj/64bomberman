#ifndef AUDIO_H
#define AUDIO_H

#include <nusys.h>
#include <nualstl_n.h>

#define AUDIO_HEAP_SIZE 0x80000
#define AUDIO_HEAP_ADDR (0x80400000 - AUDIO_HEAP_SIZE)

void Audio_init();

void Audio_play(int num);

#endif