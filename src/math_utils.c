#include "math_utils.h"
#include <math.h>
#include <stdlib.h>

float randomMinMaxf(float min, float max)
{
    return min + (float)(rand()) / ((float)(RAND_MAX / (max - min)));
}

int randomMinMax(int min, int max)
{
    return rand() % (max + 1 - min) + min;
}

static int randomComparator(const void *a, const void *b)
{
    return randomMinMax(-1, 1);
}

void randomizeArray(void *array, size_t itemSize, size_t length)
{
    qsort(array, itemSize, length, randomComparator);
}

float roundf(float num)
{
    return (num >= 0.0) ? floorf(num + 0.5f) : ceilf(num - 0.5f);
}
