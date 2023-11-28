#include <stdlib.h>

typedef struct vec2 
{
    float x, y;
} Vec2, Point2;

typedef struct vec3
{
    float x, y, z;
} Vec3, Point3;

typedef struct camera
{
    float x, y, z;
    Vec3 view_direction;
} Camera;

typedef struct circle
{
    Point3 origin;
    float radius;
    uint32_t color_code;
} Circle;

typedef struct ray
{
    Point3 origin;
    Vec3 dir;
    float t;
} Ray;