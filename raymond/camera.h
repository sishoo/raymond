#ifndef CAMERA_H
#define CAMERA_H

#include <stdlib.h>
#include "types.h"

typedef struct {
    point3 pos;
    point3 focus;
    vec3 up;
    float fov;
    int aspectRatio;
} RaymondCamera;

int initRaymondCamera(RaymondCamera *cam, float pos[3], float focus[3], float up[3], float fov, int aspectRatio) {
    cam->pos.x = pos[0];
    cam->pos.y = pos[1];
    cam->pos.z = pos[2];

    cam->focus.x = focus[0];
    cam->focus.y = focus[1];
    cam->focus.z = focus[2];

    cam->up.x = up[0];
    cam->up.y = up[1];
    cam->up.z = up[2];

    cam->fov = fov;

    cam->aspectRatio = aspectRatio;

    return 0;
}

#endif