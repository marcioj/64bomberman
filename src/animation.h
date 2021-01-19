#ifndef ANIMATION_H
#define ANIMATION_H

#include <stdbool.h>
#include "texture.h"

typedef struct Animation Animation;
typedef struct GameObject GameObject;

typedef enum AnimationDirection
{
    ANIMATION_DIRECTION_NORMAL,
    ANIMATION_DIRECTION_ALTERNATE,
} AnimationDirection;

typedef struct Animation
{
    AnimationDirection direction;
    GameObject *gameObject;
    TEXTURE *texture;
    int spriteCol;
    int spriteRow;
    int currentStep;
    int steps;
    // default step to show when animation is not playing
    // -1 will not render anything
    int defaultStep;
    // speed in ticks to change steps
    float speed;
    bool active;
    // if true you just need to call GameObject_playAnimation once to play the animation
    // otherwise GameObject_playAnimation should be called every frame
    // TODO: this is only used to handle player movement and stop the animation when no button is pressed
    // maybe instead of using this flag, it should be automatic by default and instead we should have an Animation_stop method?
    bool automatic;
    // number of times to play the animation or -1 to infinity
    int iterationCount;
    // callback which is called when the animation finished. animation is finished when iterationCount is zero
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

#endif
