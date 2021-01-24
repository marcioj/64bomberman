#ifndef ANIMATION_H
#define ANIMATION_H

#include <stdbool.h>
#include "texture.h"
#include "tile.h"

typedef struct AnimationStep
{
    float timeMs;
    Texture *texture;
    Tile *tile;
} AnimationStep;

typedef enum AnimationDirection
{
    ANIMATION_DIRECTION_NORMAL,
    ANIMATION_DIRECTION_ALTERNATE,
} AnimationDirection;

typedef struct Animation Animation;

typedef struct Animation
{
    AnimationDirection direction;
    AnimationStep *steps;
    void *caller;
    int stepsCount;
    int currentStep;
    // default step to show when animation is not playing
    // -1 will not render anything
    int defaultStep;
    bool playing;
    // if true you just need to call Animation_play once to play the animation
    // otherwise Animation_play should be called on every frame
    bool automatic;
    // number of times to play the animation or -1 to infinity
    int iterationCount;
    // callback which is called when the animation finish. animation is finished when iterationCount reaches zero
    void (*onFinish)(Animation *self);
    // private
    int _stepChangeRate;
    int _prevStepChangeRate;
    float _timeElapsed;
} Animation;

Animation *Animation_new();

void Animation_init(Animation *animation);

void Animation_render(Animation *animation, int x, int y);

void Animation_update(Animation *animation, float dt);

void Animation_reset(Animation *animation);

void Animation_play(Animation *animation);

#endif
