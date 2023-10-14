#ifndef RAYMONDTYPES_H
#define RAYMONDTYPES_H

typedef struct {
    float x;
    float y;
    float z;
} vec3, point3;

typedef struct {
    point3 origin;
    vec3 dir;
    float current;
} ray;

typedef struct {
    point3 pos;
    float radius;
} sphere;

typedef struct {
    char r;
    char g;
    char b;
} RGB;

void initSphere(sphere *obj, float x, float y, float z, float rad) {
    obj->pos.x = x; 
    obj->pos.y = y;
    obj->pos.z = z;
    obj->radius = rad;
}

#endif