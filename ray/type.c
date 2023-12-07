#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <stdio.h>

const float IMAGE_WIDTH = 800.0;
const float IMAGE_HEIGHT = 600.0;
const uint32_t NUM_OBJECTS = 200;
const uint32_t NODE_CONTENT_LIMIT = 1;
const float ROOT_NODE_X = 0.0;
const float ROOT_NODE_Y = 0.0;

typedef struct vec2
{
    float x, y;
} Vec2, Point2;

typedef struct vec3
{
    float x, y, z;
} Vec3, Point3;

typedef struct circle
{
    Point3 origin;
    float radius;
    uint32_t color_code;
} Circle;

// this is by default shifted over by 16
// when you go to deref shift it back
// if the first bit is one the node has children
// if the first bit is zero it is an object
typedef uint64_t Node;

void init_random_object(Circle *object, uint32_t seed)
{
    srand(seed);
    object->origin.x = (float)rand() / (float)(RAND_MAX / IMAGE_WIDTH);
    object->origin.y = (float)rand() / (float)(RAND_MAX / IMAGE_HEIGHT);
    object->origin.z = (float)rand() / (float)(RAND_MAX / 69.0);
    object->radius = 10.0;
    object->color_code = 0b00000000000000000000000011111111; 
}

void subdivide(Node *node_ptr, const uint32_t parent_depth)
{   
    // make sure to shift it down so that its a valid ptr
    Circle *object_ptr = (Circle *)(*node_ptr >> 16);
    Node *child_ptr = calloc(4, sizeof(Node));

    float child_width = IMAGE_WIDTH / (1 << parent_depth);
    float child_height = IMAGE_HEIGHT / (1 << parent_depth);
    uint32_t offset_table[2][2] = {{3, 1}, {2, 0}};
    bool child_contains_x = (ROOT_NODE_X <= object_ptr->origin.x) && (object_ptr->origin.x < (ROOT_NODE_X + child_width));
    bool child_contains_y = (ROOT_NODE_Y <= object_ptr->origin.y) && (object_ptr->origin.y < (ROOT_NODE_Y + child_height));
    uint32_t offset = offset_table[child_contains_x][child_contains_y];
    *(child_ptr + offset) = (Circle *)((uintptr_t)object_ptr << 16);
    *node_ptr = (Node)((uintptr_t)child_ptr << 16) | 1;
    return;
}

void insert(Node *node_ptr, Circle *object_ptr)
{
    // NOTE: at first the object ptr is a regular, non tagged pointer

    // the node passed in first will always be the root
    // that is why the depth starts at 1
    uint32_t current_depth = 1;
    while (true)
    {
        // if the node has children
        if (*node_ptr & 1)
        {
            float child_width = IMAGE_WIDTH / (1 << current_depth);
            float child_height = IMAGE_HEIGHT / (1 << current_depth);
            uint32_t offset_table[2][2] = {{3, 1}, {2, 0}};
            bool child_contains_x = (ROOT_NODE_X <= object_ptr->origin.x) && (object_ptr->origin.x < (ROOT_NODE_X + child_width));
            bool child_contains_y = (ROOT_NODE_Y <= object_ptr->origin.y) && (object_ptr->origin.y < (ROOT_NODE_Y + child_height));
            uint32_t offset = offset_table[child_contains_x][child_contains_y];
            node_ptr = *node_ptr + offset;
            continue;
        }

        if (*node_ptr == 0)
        {
            *node_ptr = ((Node)object_ptr) << 16;
            return;
        }

        // if we subdivide it means the node ptr has something and has no children
        subdivide(node_ptr, current_depth);
    }
}

int main()
{
    Node root = 0;

    for (uint32_t num = 0; num < NUM_OBJECTS; num++)
    {   
        Circle object;
        printf("On object num: %lu\n", num);
        init_random_object(&object, num);
        insert(&root, &object);
    }








    // Circle *objects = malloc(sizeof(Circle) * NUM_OBJECTS);
    // for (uint32_t offset = 0; offset < NUM_OBJECTS; offset++)
    // {
    //     Circle object = *(objects + offset);
    //     srand(offset);
    //     object.origin.x = (float)rand() / (float)(RAND_MAX / IMAGE_WIDTH);
    //     object.origin.y = (float)rand() / (float)(RAND_MAX / IMAGE_HEIGHT);
    //     object.origin.z = (float)rand() / (float)(RAND_MAX / 69.0);
    //     object.radius = 69.0;
    //     object.color_code = 0b00000000000000000000000011111111;

    //     // printf("Object number: %i\n", offset);
    //     // printf("pointer to current object: %p\n", objects + offset);
    //     double start_time = (double)clock() / CLOCKS_PER_SEC;
    //     insert(&root,);
    //     double end_time = (double)clock() / CLOCKS_PER_SEC;
    //     total_time += end_time - start_time;
    //     // printf("time elapsed: %lf\n", end_time - start_time);
    //     // printf("\n");
    // }

    

    // for (uint32_t i = 0; i < NUM_OBJECTS; i++)
    // {
    //     Circle object = {};
    //     Point3 origin = {};
    //     // origin.x = 400.0;
    //     // origin.y = 300.0;
    //     // origin.z = 10.0;
    //     // object.origin = origin;
    //     // object.radius = 10.0;

    //     srand(i);
    //     origin.x = (float)rand() / (float)(RAND_MAX / IMAGE_WIDTH);
    //     origin.y = (float)rand() / (float)(RAND_MAX / IMAGE_HEIGHT);
    //     origin.z = (float)rand() / (float)(RAND_MAX / 69.0);
    //     object.origin = origin;
    //     object.radius = 10.0;

    //     object.color_code = 0b00000000000000000000000011111111;

    //     printf("iteration: %i\n", i);
    //     printf("pointer to current object: %p\n", &object);
    //     float start_time = (float)clock() / CLOCKS_PER_SEC;
    //     insert(&root, &object);
    //     float end_time = (float)clock() / CLOCKS_PER_SEC;
    //     printf("time elapsed: %f\n", end_time - start_time);
    // }
    printf("didnt get crapped on\n");
    return 0;
}
