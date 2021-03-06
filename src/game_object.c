#include "game_object.h"
#include <malloc.h>

void GameObject_render(GameObject *gameObject)
{
    Animation *currentAnimation = &gameObject->animations[gameObject->currentAnimationIndex];
    Animation_render(currentAnimation, gameObject->x, gameObject->y);
}

GameObject *GameObject_new()
{
    GameObject *gameObject = malloc(sizeof(GameObject));
    gameObject->currentAnimationIndex = 0;
    gameObject->data = NULL;
    gameObject->type = -1;
    gameObject->animations = NULL;
    gameObject->animationsCount = 0;
    gameObject->render = GameObject_render;
    gameObject->update = NULL;
    gameObject->destroy = NULL;
    gameObject->onCollisionEnter = NULL;
    gameObject->onCollisionExit = NULL;
    gameObject->zIndex = 0;
    gameObject->_prevCollisions = malloc(sizeof(GameObjectArray));
    vec_init(gameObject->_prevCollisions);
    gameObject->_newCollisions = malloc(sizeof(GameObjectArray));
    vec_init(gameObject->_newCollisions);
    return gameObject;
}

void GameObject_destroy(GameObject *gameObject)
{
    if (!gameObject)
        return;
    if (gameObject->destroy)
        gameObject->destroy(gameObject);
    if (gameObject->data)
        free(gameObject->data);
    for (size_t i = 0; i < gameObject->animationsCount; i++)
    {
        Animation *animation = gameObject->animations + i;
        free(animation->steps);
    }
    if (gameObject->animations)
        free(gameObject->animations);

    // remove this game object reference from objects which this object collided before
    for (size_t i = 0; i < gameObject->_prevCollisions->length; i++)
    {
        GameObjectArray *prevCollisions = gameObject->_prevCollisions->data[i]->_prevCollisions;
        vec_remove(prevCollisions, gameObject);
    }

    vec_deinit(gameObject->_prevCollisions);
    free(gameObject->_prevCollisions);
    vec_deinit(gameObject->_newCollisions);
    free(gameObject->_newCollisions);
    free(gameObject);
}

void GameObject_update(GameObject *gameObject, float dt)
{
    Animation *currentAnimation = &gameObject->animations[gameObject->currentAnimationIndex];
    Animation_update(currentAnimation, dt);
    if (gameObject->update)
        gameObject->update(gameObject, dt);
}

Animation *GameObject_playAnimation(GameObject *gameObject, int animationIndex)
{
    if (gameObject->animations == NULL)
        return NULL;

    if (gameObject->currentAnimationIndex != animationIndex)
    {
        Animation *prevAnimation = &gameObject->animations[gameObject->currentAnimationIndex];
        Animation_reset(prevAnimation);
    }

    gameObject->currentAnimationIndex = animationIndex;
    Animation *currentAnimation = &gameObject->animations[gameObject->currentAnimationIndex];
    currentAnimation->caller = gameObject;
    Animation_play(currentAnimation);
    return currentAnimation;
}
