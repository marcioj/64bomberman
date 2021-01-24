#include <string.h>
#include <malloc.h>
#include "animation.h"
#include "screen_printf.h"

Animation *Animation_new()
{
    Animation *animation = malloc(sizeof(Animation));
    Animation_init(animation);
    return animation;
}

void Animation_init(Animation *animation)
{
    animation->direction = ANIMATION_DIRECTION_NORMAL;
    animation->steps = NULL;
    animation->stepsCount = 0;
    animation->defaultStep = -1;
    animation->iterationCount = 1;
    animation->playing = false;
    animation->automatic = true;
    animation->_timeElapsed = 0;
    animation->currentStep = 0;
    animation->onFinish = NULL;
    animation->_stepChangeRate = 1;
    animation->_prevStepChangeRate = 0;
}

void Animation_reset(Animation *animation)
{
    animation->playing = false;
    animation->_timeElapsed = 0;
    animation->currentStep = 0;
    animation->onFinish = NULL;
    animation->_stepChangeRate = 1;
    animation->_prevStepChangeRate = 0;
}

void Animation_play(Animation *animation)
{
    animation->playing = true;
}

void Animation_render(Animation *animation, int x, int y)
{
    int currentStep = animation->playing ? animation->currentStep : animation->defaultStep;
    if (currentStep == -1 || !animation->steps)
        return;

    AnimationStep *step = animation->steps + currentStep;
    if (step->texture)
        Texture_render(step->texture, x, y);
    if (step->tile)
        Tile_render(step->tile, x, y);
}

void Animation_update(Animation *animation, float dt)
{
    if (!animation->playing || !animation->steps)
        return;

    if (!animation->automatic)
        animation->playing = false;

    AnimationStep *step = animation->steps + animation->currentStep;
    bool wasLastStep = animation->currentStep == animation->stepsCount - 1;
    bool wasFirstStep = animation->currentStep == 0;
    bool stepChanged = false;
    animation->_timeElapsed += dt;
    // change currentStep when time passes
    if (animation->_timeElapsed >= step->timeMs)
    {
        animation->currentStep += animation->_stepChangeRate;

        if (animation->currentStep == animation->stepsCount)
            animation->currentStep = 0;
        else if (animation->currentStep == -1)
            animation->currentStep = animation->stepsCount - 1;

        animation->_timeElapsed = 0;
        stepChanged = true;
    }
    wasLastStep &= stepChanged;
    wasFirstStep &= stepChanged;

    bool hasFinishedIteration = false;
    bool isFirstStep = animation->currentStep == 0;
    bool isLastStep = animation->currentStep == animation->stepsCount - 1;

    if (animation->direction == ANIMATION_DIRECTION_NORMAL)
    {
        hasFinishedIteration = wasLastStep;
    }
    else if (animation->direction == ANIMATION_DIRECTION_ALTERNATE)
    {
        hasFinishedIteration = wasFirstStep && animation->_prevStepChangeRate == -1;
        // we are moving back and just finished the first step
        if (isLastStep && stepChanged && animation->_stepChangeRate == 1)
        {
            animation->_prevStepChangeRate = animation->_stepChangeRate;
            animation->_stepChangeRate = -1;
        }
        else if (isFirstStep && stepChanged && animation->_stepChangeRate == -1)
        {
            animation->_prevStepChangeRate = animation->_stepChangeRate;
            animation->_stepChangeRate = 1;
        }
    }

    if (hasFinishedIteration && animation->iterationCount != -1)
        animation->iterationCount--;

    // animation finished
    if (hasFinishedIteration && animation->iterationCount == 0)
    {
        if (animation->onFinish)
            animation->onFinish(animation);
        Animation_reset(animation);
    }
}
