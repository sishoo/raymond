#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <stdio.h>

const float IMAGE_WIDTH = 800.0;
const float IMAGE_HEIGHT = 600.0;
const uint32_t NUM_OBJECTS = 2;
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

// element is by default shifted over by 16
// when you go to deref shift it back
// if the first bit is one the node has children
typedef struct node
{
    uint64_t element;
} Node;

void init_root_node(Node *node)
{
    node->element = 0ULL;
}

void init_blank_node(Node *node)
{
    node->element = 0ULL;
}

void subdivide(Node *node, const uint32_t parent_depth)
{
    printf("SFSF\n");
    Circle *object = node->element;
    Node *children = malloc(sizeof(Node) * 4);
    init_blank_node(children);
    init_blank_node(children + 1);
    init_blank_node(children + 2);
    init_blank_node(children + 3);
    node = children;

    float child_width = IMAGE_WIDTH / (1 << parent_depth);
    float child_height = IMAGE_HEIGHT / (1 << parent_depth);
    uint32_t offset_table[2][2] = {{3, 1}, {2, 0}};
    bool child_contains_x = (ROOT_NODE_X <= object->origin.x) && (object->origin.x < (ROOT_NODE_X + child_width));
    bool child_contains_y = (ROOT_NODE_Y <= object->origin.y) && (object->origin.y < (ROOT_NODE_Y + child_height));
    uint32_t offset = offset_table[child_contains_x][child_contains_y];
    (children + offset)->element = object;
    return;
}

void insert(Node *node, Circle *object)
{
    Node *current_node = node;
    uint32_t current_depth = 1; // depth of the node we are on
    while (true)
    {
        // if the node has children
        if (current_node->element & 1)
        {
            float child_width = IMAGE_WIDTH / (1 << current_depth);
            float child_height = IMAGE_HEIGHT / (1 << current_depth);
            uint32_t offset_table[2][2] = {{3, 1}, {2, 0}};
            bool child_contains_x = (ROOT_NODE_X <= object->origin.x) && (object->origin.x < (ROOT_NODE_X + child_width));
            bool child_contains_y = (ROOT_NODE_Y <= object->origin.y) && (object->origin.y < (ROOT_NODE_Y + child_height));
            uint32_t offset = offset_table[child_contains_x][child_contains_y];
            current_node =  (current_node->element >> 16);
            continue;
        }

        if (current_node->element == 0)
        {
            current_node->element = (uint64_t)object << 16;
            return;
        }

        subdivide(current_node, current_depth);
    }
}

int main()
{
    Node root;
    init_root_node(&root);

    double total_time = 0;

    for (uint32_t num = 0; num < NUM_OBJECTS; num++)
    {
        Circle object = {};
        srand(num);
        object.origin.x = (float)rand() / (float)(RAND_MAX / IMAGE_WIDTH);
        object.origin.y = (float)rand() / (float)(RAND_MAX / IMAGE_HEIGHT);
        object.origin.z = (float)rand() / (float)(RAND_MAX / 69.0);
        object.radius = 10.0;
        object.color_code = 0b00000000000000000000000011111111;

        printf("Object number: %i\n", num);
        double start_time = (double)clock() / CLOCKS_PER_SEC;
        insert(&root, &object);
        double end_time = (double)clock() / CLOCKS_PER_SEC;
        total_time += end_time - start_time;
        printf("time elapsed: %lf\n\n", end_time - start_time);
    }

    printf("total time elapsed for %lu objects: %lf\n", NUM_OBJECTS, total_time);






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

    return 0;
}
