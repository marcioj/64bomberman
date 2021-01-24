#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "game_object.h"
#include <vec.h>
#include "game_object_type.h"
#include "animation.h"

typedef struct GameObject GameObject;

typedef vec_t(GameObject *) GameObjectArray;

typedef struct GameObject
{
    GameObjectType type;
    float x;
    float y;
    float prevX;
    float prevY;
    int currentAnimationIndex;
    Animation *animations;
    int animationsCount;
    void (*render)(GameObject *self);
    void (*update)(GameObject *self, float dt);
    void (*destroy)(GameObject *self);
    void (*onCollisionEnter)(GameObject *self, GameObject *collided);
    void (*onCollisionExit)(GameObject *self, GameObject *collided);
    void *data;
    int zIndex;
    // private
    GameObjectArray *_prevCollisions;
    GameObjectArray *_newCollisions;
} GameObject;

GameObject *GameObject_new();

void GameObject_destroy(GameObject *gameObject);

Animation *GameObject_playAnimation(GameObject *gameObject, int animationIndex);

void GameObject_update(GameObject *gameObject, float dt);

#endif
