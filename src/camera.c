#include "camera.h"
#include "level.h"
#include "constants.h"
#include "math_utils.h"

void Camera_update(Camera *camera)
{
    int bgPositionX = 0;
    int bgPositionY = 0;

    // background position
    if (Level_getWidth() > SCREEN_WD)
    {
        int halfWidth = SCREEN_WD / 2;
        int playerCenterX = camera->targetX + TILE_SIZE / 2;
        bgPositionX = MAX(0, playerCenterX - halfWidth); // ignore negative offset

        if (playerCenterX + halfWidth >= Level_getWidth())
        {
            bgPositionX = Level_getWidth() - SCREEN_WD;
        }
    }

    camera->bgPositionX = bgPositionX;
    camera->bgPositionY = bgPositionY;
    Level_setBackgroundPosition(camera->bgPositionX, camera->bgPositionY);

    // player offset
    int playerPositionX = camera->targetX;
    int playerPositionY = camera->targetY;
    if (Level_getWidth() > SCREEN_WD)
    {
        int halfWidth = SCREEN_WD / 2;
        int cameraCenterX = camera->targetX + TILE_SIZE / 2;
        // player passed the middle of screen
        if (cameraCenterX - halfWidth > 0)
        {
            // lock position in middle of screen
            playerPositionX = halfWidth - TILE_SIZE / 2;
        }

        // player reached the right side of the background
        if (cameraCenterX + halfWidth >= Level_getWidth())
        {
            int xOffset = cameraCenterX + halfWidth - Level_getWidth();
            playerPositionX += xOffset;
        }
    }
    camera->targetScreenX = playerPositionX;
    camera->targetScreenY = playerPositionY;
}
