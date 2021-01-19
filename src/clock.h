#ifndef CLOCK_H
#define CLOCK_H

unsigned long long Clock_ticks();

float Clock_timeMs();

void Clock_nextTick();

float Clock_deltaTime();

float Clock_lastFrameTime();

#endif
