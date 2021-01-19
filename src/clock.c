#include "clock.h"

#ifdef _ULTRA64
#include <nusys.h>
#else
#include <sys/time.h>
#include <stdlib.h>
#include <stdint.h>
#endif

float lastFrameTime;
float deltaTime;
unsigned long long ticks;

float Clock_deltaTime()
{
    return deltaTime;
}

float Clock_lastFrameTime()
{
    return lastFrameTime;
}

unsigned long long Clock_ticks()
{
    return ticks;
}

#ifdef _ULTRA64
float Clock_timeMs()
{
    return (float)OS_CYCLES_TO_USEC(osGetTime()) / 1000000.0f;
}
#else
float Clock_timeMs()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    int time_in_usec = tv.tv_sec * (uint64_t)1000000 + tv.tv_usec;
    return time_in_usec / 1000000.0f;
}
#endif

void Clock_nextTick()
{
    float newTime = Clock_timeMs();
    deltaTime = newTime - lastFrameTime;
    lastFrameTime = newTime;
    ticks++;
}
