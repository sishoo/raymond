#ifndef RAY_H
#define RAY_H

#include "types.h"
#include "camera.h"
#include "utils.h"
#include <math.h>

typedef struct {
    point3 origin;
    vec3 dir;
    float current;
} ray;

ray fromCameraTo(RaymondCamera *cam, int x, int y, int imgWidth, int imgHeight) {
    float normX = (x + 0.5) / imgWidth;
    float normY = (y + 0.5) / imgHeight;

    float mappedX = 2 * normX - 1;
    float mappedY = 1 - 2 * normY;

    vec3 rayDir;
    rayDir.x = mappedX * tan(cam->fov / 2);
    rayDir.y = mappedY * tan(cam->fov / 2) * cam->aspectRatio;
    rayDir.z = -1;

    normalizeVP3(&rayDir);

    ray raymond;
    raymond.origin = cam->pos;
    raymond.dir = rayDir;
    return raymond;
}

RGB traceRay(ray raymond, point3 light, sphere *objs) {
    
    intersectionRes intersection = findNearestIntersection(raymond, scene);




    return color;
}

#endif