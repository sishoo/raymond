#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <types.h>


const float IMAGE_WIDTH = 800.0;
const float IMAGE_HEIGHT = 600.0;
const uint32_t NUM_OBJECTS = 1;
const uint32_t BACKGROUND_COLOR = 0b0;
const uint32_t NODE_CONTENT_LIMIT = 4;

// const float CAMERA_X = IMAGE_WIDTH / 2.0;
// const float CAMERA_Y = IMAGE_HEIGHT / 2.0;

// const float CAMERA_Z = 0.0;

/*
    kind of wack
    if x is -1 than element is a pointer to the first child
    and y is the depth of the current node

    if x is not -1 then element is a pointer to the circle object
    and x and y are the origin of the aabb

    kind of like cpu opcodes
*/

typedef struct node
{
    uint32_t element;
    float x, y;
} Node;

void init_root_node(Node *node)
{
    node->element = NULL;
    node->x = IMAGE_WIDTH;
    node->y = IMAGE_HEIGHT;
}

void subdivide(Node *node, const uint32_t parent_depth)
{
    float bounds_x = node->x;
    float bounds_y = node->y;
    node->x = -1;
    node->y = (float)parent_depth;

    uint32_t new_depth = parent_depth + 1;
    float new_width = IMAGE_WIDTH / new_depth;
    float new_height = IMAGE_HEIGHT / new_depth;

    /*
    base ptr = nw
    +1 = ne
    +2 = sw
    +3 = se
    */
    Node* children_memory = (Node *)malloc(sizeof(Node) * 4);

    uint32_t origin_offset_table_x[] = {1, 2, 1, 2};
    uint32_t origin_offset_table_y[] = {1, 1, 2, 2};
    for (uint32_t offset = 0; offset < 4; offset++)
    {
        Node node = *(children_memory + offset);
        node.x = bounds_x * *(origin_offset_table_x + offset);
        node.y = bounds_y * *(origin_offset_table_y + offset);
        node.element = NULL;
    }
    node->element = children_memory;
}

void insert(Node *node, Circle *object)
{
    Node *current = node;
    uint32_t depth = 1;
    while (true)
    {
        if (node->x == -1)
        {
            for (uint32_t offset = 0; offset < 4; offset++)
            {
                Node *child = (Node *)(current->element + offset);

                float child_bounds_x = child->x;
                float child_bounds_y = child->y;
                float child_bounds_width = IMAGE_WIDTH / node->y;
                float child_bounds_height = IMAGE_HEIGHT / node->y;

                bool child_x_contains = child_bounds_x <= object->origin.x && object->origin.x < child_bounds_x + child_bounds_width;
                bool child_y_contains = child_bounds_y <= object->origin.y && object->origin.y < child_bounds_y + child_bounds_height;

                if (child_x_contains && child_y_contains)
                {
                    current = child;
                    depth++;
                    break;
                }
            }
            continue;
        }

        if (!node->element)
        {
            node->element = (uint32_t)object;
            return;
        }

        subdivide(node, depth);
    }
}

// void cleanup(Node *root)
// {
//     Node *current = root;
//     while (true)
//     {
//         if (root->x == -1)
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

uint32_t cast_ray(
    Ray ray,
    Circle *objects,
    uint32_t num_objects)
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