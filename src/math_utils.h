#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#include <math.h>
#include <stddef.h>

#ifdef _ULTRA64
#include <PR/gu.h>
#endif

#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

float randomMinMaxf(float min, float max);

int randomMinMax(int min, int max);

float roundf(float num);

void randomizeArray(void *array, size_t itemSize, size_t length);

#endif
