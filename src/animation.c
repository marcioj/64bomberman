#include <string.h>
#include <malloc.h>
#include "animation.h"
#include "constants.h"
#include "texture.h"
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
    animation->spriteCol = 0;
    animation->spriteRow = 0;
    animation->steps = 1;
    animation->defaultStep = 0;
    animation->speed = 0.16f;
    animation->automatic = false;
    animation->iterationCount = -1; // infinity
    animation->active = false;
    animation->_timeElapsed = 0;
    animation->currentStep = 0;
    animation->onFinish = NULL;
    animation->_stepChangeRate = 1;
    animation->_prevStepChangeRate = 0;
    animation->gameObject = NULL;
}

void Animation_reset(Animation *animation)
{
    animation->active = false;
    animation->_timeElapsed = 0;
    animation->currentStep = 0;
    animation->onFinish = NULL;
    animation->_stepChangeRate = 1;
    animation->_prevStepChangeRate = 0;
    animation->gameObject = NULL;
}

#ifdef _ULTRA64
#include "graphic.h"

void Animation_render(Animation *animation, int x, int y)
{
    uObjBg *objBg = spritePtr++;
    memcpy(objBg, animation->texture, sizeof(uObjBg));

    int currentStep = animation->active ? animation->currentStep : animation->defaultStep;
    if (currentStep == -1)
        return;

    objBg->s.imageX = (int)((animation->spriteCol + currentStep) * TILE_SIZE) << 5;
    objBg->s.imageY = (int)(animation->spriteRow * TILE_SIZE) << 5;
    objBg->s.frameW = TILE_SIZE << 2;
    objBg->s.frameH = TILE_SIZE << 2;
    objBg->s.frameX = x << 2;
    objBg->s.frameY = (SCREEN_TOP_OFFSET + y) << 2;
    objBg->s.scaleW = (1 << 10);
    objBg->s.scaleH = (1 << 10);

    // prevent image underflow
    if (objBg->s.frameX < -objBg->s.frameW || objBg->s.frameY < -objBg->s.frameH)
        return;

    gDPSetCycleType(displayListPtr++, G_CYC_1CYCLE);
    gSPBgRect1Cyc(displayListPtr++, objBg);
    gDPPipeSync(displayListPtr++);

    osWritebackDCache(objBg, sizeof(uObjBg));
}
#else
void Animation_render(Animation *animation, int x, int y)
{
    int currentStep = animation->active ? animation->currentStep : animation->defaultStep;
    if (currentStep == -1)
        return;

    SDL_Rect source = {
        .h = TILE_SIZE,
        .w = TILE_SIZE,
        .x = (int)((animation->spriteCol + currentStep) * TILE_SIZE),
        .y = (int)(animation->spriteRow * TILE_SIZE),
    };
    SDL_Rect dest = {
        .h = source.h,
        .w = source.w,
        .x = x,
        .y = SCREEN_TOP_OFFSET + y,
    };

    Texture_render(animation->texture, source, dest, 0);
}
#endif

void Animation_update(Animation *animation, float dt)
{
    if (!animation->active)
        return;

    bool wasLastStep = animation->currentStep == animation->steps - 1;
    bool wasFirstStep = animation->currentStep == 0;
    bool stepChanged = false;
    animation->_timeElapsed += dt;
    // change currentStep when time passes
    if (animation->_timeElapsed >= animation->speed)
    {
        animation->currentStep += animation->_stepChangeRate;

        if (animation->currentStep == animation->steps)
            animation->currentStep = 0;
        else if (animation->currentStep == -1)
            animation->currentStep = animation->steps - 1;

        animation->_timeElapsed = 0;
        stepChanged = true;
    }
    wasLastStep &= stepChanged;
    wasFirstStep &= stepChanged;

    if (!animation->automatic)
        animation->active = false;

    bool hasFinishedIteration = false;
    bool isFirstStep = animation->currentStep == 0;
    bool isLastStep = animation->currentStep == animation->steps - 1;

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
