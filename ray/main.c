#include <stdlib.h>
#include <stdio.h>
#include <math.h>

const uint32_t IMAGE_WIDTH = 800;
const uint32_t IMAGE_HEIGHT = 600;

uint32_t write_data_to_image(uint32_t **image_data) {
    // p6 format

    FILE* file_ptr = fopen("./image.ppm", "w");

    if (file_ptr == NULL) {
        printf("Cannot open the file to be written to.");
        return -1;
    }


    uint32_t image_width_string_size = (uint32_t) ((ceil(log10(IMAGE_WIDTH)))+1)*sizeof(char));
    uint32_t image_height_string_size = (uint32_t)((ceil(log10(IMAGE_HEIGHT))+1)*sizeof(char));
    char *image_width_string = (char *)malloc(sizeof(char *) * image_width_string_size);
    char *image_height_string = (char *)malloc(sizeof(char *) * image_height_string_size);

    sprintf(image_width_string, "%lu", IMAGE_WIDTH);
    sprintf(image_height_string, "%lu", IMAGE_HEIGHT);

    fprintf(file_ptr, "P6 ");
    fprintf(file_ptr, image_width_string);
    fprintf(file_ptr, image_height_string);
    fprintf(file_ptr, "255\n");

    for (uint32_t y = 0; y < IMAGE_HEIGHT; y++) {
        fwrite(*(image_data + y), 32, IMAGE_WIDTH * 3, file_ptr);
    }

    return 0;
}

int main() {

    uint32_t **image_data = (uint32_t **)malloc(IMAGE_HEIGHT * sizeof(uint32_t *));
    for (uint32_t i = 0; i < IMAGE_HEIGHT; i++) {
        image_data[i] = (uint32_t *)malloc(3 * IMAGE_WIDTH * sizeof(uint32_t));
    }
    
    for (uint32_t y = 0; y < IMAGE_HEIGHT; y++) 
    {
        for (uint32_t x = 0; x < IMAGE_WIDTH / 3; x++) 
        {
            image_data[y][x] = 0;
            image_data[y][x + 1] = 0;
            image_data[y][x + 2] = 0;
        }
    }

    uint32_t result = write_data_to_image(image_data);

    if (result < 0) {
        return -1;
    }
    





    return 0;
}