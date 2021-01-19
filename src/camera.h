#ifndef CAMERA_H
#define CAMERA_H

typedef struct Camera
{
    int targetX;
    int targetY;
    int bgPositionX;
    int bgPositionY;
    int targetScreenX;
    int targetScreenY;
} Camera;

void Camera_update(Camera *camera);

extern Camera camera;

#endif
