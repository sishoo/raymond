#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <stdio.h>

const float IMAGE_WIDTH = 800.0;
const float IMAGE_HEIGHT = 600.0;
const uint32_t NUM_OBJECTS = 100;
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

typedef struct node
{
    uint32_t element;
    char has_children;
} Node;

typedef struct node_pool
{
    uint32_t right, left, offset;
} NodePool;

void init_root_node(Node *node)
{
    node->element = NULL;
    node->has_children = 0;
}

void init_blank_node(Node *node)
{
    node->element = NULL;
    node->has_children = 0;
}

void subdivide(Node *node, const uint32_t parent_depth)
{
    Circle *object = (Circle *)node->element;
    node->has_children = 1;

    Node *children = malloc(sizeof(Node) * 4);
    init_blank_node(children);
    init_blank_node(children + 1);
    init_blank_node(children + 2);
    init_blank_node(children + 3);
    node->element = (uint32_t)children;

    for (uint32_t offset = 0; offset < 4; offset++)
    {
        Node *child = (children + offset);
        float child_width = IMAGE_WIDTH / (1 << parent_depth);
        float child_height = IMAGE_HEIGHT / (1 << parent_depth);
        float child_x = ROOT_NODE_X + child_width * ((1 << parent_depth) - 1);
        float child_y = ROOT_NODE_Y + child_height * ((1 << parent_depth) - 1);


        bool child_contains_x = (child_x <= object->origin.x) && (object->origin.x < (child_x + child_width));
        bool child_contains_y = (child_y <= object->origin.y) && (object->origin.y < (child_y + child_height));
       
        if (child_contains_x && child_contains_y)
        {
            child->element = object;
            return;
        }
    }
}

void insert(Node *node, Circle *object)
{
    Node *current = node;
    uint32_t current_depth = 1; // depth of the node we are on
    while (true)
    {
        // if the node has children
        if (1 == node->has_children)
        {
            // we loop through each of the children
            for (uint32_t offset = 0; offset < 4; offset++)
            {
                Node *child = (Node *)(current->element + offset);
                float child_width = IMAGE_WIDTH / (1 << current_depth);
                float child_height = IMAGE_HEIGHT / (1 << current_depth);
                float child_x = ROOT_NODE_X + child_width * ((1 << current_depth) - 1);
                float child_y = ROOT_NODE_Y + child_height * ((1 << current_depth) - 1);

                bool child_contains_x = (child_x <= object->origin.x) && (object->origin.x < (child_x + child_width));
                bool child_contains_y = (child_y <= object->origin.y) && (object->origin.y < (child_y + child_height));

                if (child_contains_x && child_contains_y)
                {
                    current = child;
                    current_depth++;
                    break;
                }
            }
            continue;
        }

        if (NULL == node->element)
        {
            node->element = (uint32_t)object;
            return;
        }

        subdivide(current, current_depth);
    }
}

int main()
{
    Node root;
    init_root_node(&root);

    for (uint32_t i = 0; i < NUM_OBJECTS; i++)
    {
        Circle object = {};
        Point3 origin = {};
        // origin.x = 400.0;
        // origin.y = 300.0;
        // origin.z = 10.0;
        // object.origin = origin;
        // object.radius = 10.0;

        srand(i);
        origin.x = (float)rand() / (float)(RAND_MAX / IMAGE_WIDTH);
        origin.y = (float)rand() / (float)(RAND_MAX / IMAGE_HEIGHT);
        origin.z = (float)rand() / (float)(RAND_MAX / 69.0);
        object.origin = origin;
        object.radius = 10.0;

        object.color_code = 0b00000000000000000000000011111111;

        printf("iteration: %i\n", i);
        printf("pointer to current object: %p\n", &object);
        float start_time = (float)clock() / CLOCKS_PER_SEC;
        insert(&root, &object);
        float end_time = (float)clock() / CLOCKS_PER_SEC;
        printf("time elapsed: %f\n", end_time - start_time);
    }

    return 0;
}
