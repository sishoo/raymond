#ifndef UTILS_H
#define UTILS_H

#include "types.h"
#include "math.h"

void normalizeVP3(vec3 *vec) {
    float len = sqrt(vec->x * vec->x + vec->y * vec->y + vec->z * vec->z);

    if (0 != len) {
        vec->x /= len;
        vec->y /= len;
        vec->z /= len;
    }
}

#endif