#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <types.h>


const float IMAGE_WIDTH = 800.0;
const float IMAGE_HEIGHT = 600.0;
const uint32_t NUM_OBJECTS = 1;
const uint32_t BACKGROUND_COLOR = 0b0;
const uint32_t NODE_CONTENT_LIMIT = 1;

const float ROOT_NODE_X = 0.0;
const float ROOT_NODE_Y = 0.0;

// const float CAMERA_X = IMAGE_WIDTH / 2.0;
// const float CAMERA_Y = IMAGE_HEIGHT / 2.0;

// const float CAMERA_Z = 0.0;

/*
    kind of wack
    if x is -1 than element is a pointer to the first child
    and y is the depth of the current node

    if x is not -1 then element is a pointer to the circle object
    and x and y are the origin of the aabb

    kind of like cisc instructions
*/

// 96 bits
// typedef struct node
// {
//     uint32_t element;
//     float x, y;
// } Node;

typedef struct node
{
    uint32_t element;
    char sign;
} Node;

void init_root_node(Node *node)
{
    node->element = NULL;
    node->sign = -1;
}

void init_blank_node(Node *node)
{
    node->element = NULL;
    node->sign = 0;
}

void get(Node *root, Point2 point)
{
    Node *current = root;
    while (true)
    {
        if (-1 == current->sign)
        {

        }

        return current->element;






    }
}

void subdivide(Node** leaves, Node *node, const uint32_t parent_depth)
{
    Circle* object = (Circle *)node->element;
    node->sign = -1;

    Node* children = (Node *)malloc(sizeof(Node) * 4);
    init_blank_node(children);
    init_blank_node(children + 1);
    init_blank_node(children + 2);
    init_blank_node(children + 3);
    node->element = children;

    for (uint32_t offset = 0; offset < 4; offset++)
    {
        Node *child = (Node *)(children + offset);
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
        if (-1 == node->sign)
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
            node->sign = 1;
            return;
        }

        subdivide(current, current_depth);
    }
}

// void cleanup(Node *root)
// {
//     Node *current = root;
//     while (true)
//     {
//         if (root->sign == -1)
//         {
//             for (uint32_t offset = 0; offset < 4; offset++)
//             {
//                 Node *child = (Node *)(current->element + offset);

//                 if (!child->element)
//                 {
                    
//                 }
//             }
//         } 
//     }
// }







float get_euclidian_distance(Point3 a, Point3 b)
{
    float x = a.x * a.x + b.x * b.x;
    float y = a.y * a.y + b.y * b.y;
    float z = a.z * a.z + b.z * b.z;
    return sqrt(x + y + z);
}

float get_magnitude(Vec3 vec)
{
    return sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}


// uint32_t write_data_to_image(uint32_t **image_data)
// {
//     FILE* file_ptr = fopen("./image.ppm", "w");

//     if (file_ptr == NULL) {
//         printf("Cannot open the file to be written to.");
//         return -1;
//     }

//     uint32_t image_width_string_size = (uint32_t)((ceil(log10(IMAGE_WIDTH))+1)*sizeof(char));
//     uint32_t image_height_string_size = (uint32_t)((ceil(log10(IMAGE_HEIGHT))+1)*sizeof(char));

//     char *image_width_string = (char *)malloc(sizeof(char *) * image_width_string_size);
//     char *image_height_string = (char *)malloc(sizeof(char *) * image_height_string_size);

//     sprintf(image_width_string, "%lu ", IMAGE_WIDTH);
//     sprintf(image_height_string, "%lu", IMAGE_HEIGHT);

//     fprintf(file_ptr, "P6 ");
//     fprintf(file_ptr, image_width_string);
//     fprintf(file_ptr, image_height_string);
//     fprintf(file_ptr, "255\n");

//     for (uint32_t y = 0; y < IMAGE_HEIGHT; y++) {
//         fwrite(*(image_data + y), 32, IMAGE_WIDTH * 3, file_ptr);
//     }

//     return 0;
// }

Vec3 subtract_VP3(Vec3 a, Vec3 b)
{
    Vec3 new_vec = {};

    new_vec.x = a.x - b.x;
    new_vec.y = a.y - b.y;
    new_vec.z = a.z - b.z;

    return new_vec;
}

float dot_VP3(Vec3 a, Vec3 b)
{
    // v = ai + bj + ck and w = di + ej + fk
    // v . w = ad + be + cf
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vec3 scale_VP3(Vec3 vec, float scale)
{
    Vec3 new_vec = {};
    new_vec.x = vec.x * scale;
    new_vec.y = vec.y * scale;
    new_vec.z = vec.z * scale;

    return new_vec;
}

bool object_contains_ray(Point3 ray_origin, Point3 object_origin, float object_radius)
{
    bool x_contains = (object_origin.x - object_radius) < ray_origin.x && ray_origin.x < (object_origin.x + object_radius);
    bool y_contains = (object_origin.y - object_radius) < ray_origin.y && ray_origin.y < (object_origin.y + object_radius);
    bool z_contains = (object_origin.z - object_radius) < ray_origin.z && ray_origin.z < (object_origin.z + object_radius);
    return x_contains && y_contains && z_contains;
}

uint32_t cast_ray(
    Ray ray,
    Circle *objects,
    uint32_t num_objects,
    Light *lights
    uint32_t num_lights)
{
    uint32_t color_code = 0b00000000000000000000000000000000;
    for (uint32_t i = 0; i < num_objects; i++)
    {
        Circle object = objects[i];
        Vec3 L = subtract_VP3(object.origin, ray.origin);
        Vec3 tca = scale_VP3(ray.dir, dot_VP3(L, ray.dir));
        float d = get_euclidian_distance(tca, object.origin);
        if (object.radius < d)
        {
            continue;
        }
        float thc = sqrt(object.radius * object.radius - d * d);

    }
    return color_code;
}

int main()
{
    uint32_t **image_data = (uint32_t **)malloc(IMAGE_HEIGHT * sizeof(uint32_t *));
    for (uint32_t i = 0; i < IMAGE_HEIGHT; i++)
    {
        image_data[i] = (uint32_t *)calloc(3 * IMAGE_WIDTH, sizeof(uint32_t));
    }

    // uint32_t result = write_data_to_image(image_data);

    // if (result < 0) {
    //     return -1;
    // }

    Node quadtree_root = {};
    init_root_node(&quadtree_root);


    float min_z = 99999999999999999999.0;
    Circle *objects = (Circle *)malloc(sizeof(Circle) * NUM_OBJECTS);
    for (uint32_t i = 0; i < NUM_OBJECTS; i++)
    {
        Circle object = {};
        Point3 origin = {};
        origin.x = 400.0;
        origin.y = 300.0;
        origin.z = 10.0;
        object.origin = origin;
        object.radius = 10.0;
        object.color_code = 0b00000000000000000000000011111111;
        objects[i] = object;

        if (origin.z < min_z)
        {
            min_z = origin.z;
        }
    }

    for (uint32_t y = 0; y < IMAGE_HEIGHT; y++)
    {
        for (uint32_t x = 0; x < IMAGE_WIDTH * 3; x += 3)
        {
            Ray ray = {};
            Point3 origin = {};
            Vec3 dir = {};

            origin.x = CAMERA_X;
            origin.y = CAMERA_Y;
            origin.z = CAMERA_Z;

            dir.x = (x - CAMERA_X);
            dir.y = (y - CAMERA_Y);
            dir.z = (1 - CAMERA_Z);

            ray.t = 0.0;

            uint32_t color_code = cast_ray(ray, objects, NUM_OBJECTS);
            image_data[y][x] = (color_code & 0b00000000111111110000000000000000) >> 16;
            image_data[y][x + 1] = (color_code & 0b00000000000000001111111100000000) >> 8;
            image_data[y][x + 2] = color_code & 0b00000000000000000000000011111111;
        }
    }

    return 0;
}
