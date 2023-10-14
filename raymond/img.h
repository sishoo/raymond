#ifndef IMG_H
#define IMG_H

#include "types.h"
#include <stdlib.h>

typedef struct {
    int w;
    int h;
    RGB **vals;
} RaymondImage;

int initRaymondImg(RaymondImage *img, int w, int h) {
    img->w = w;
    img->h = h;

    img->vals = (RGB **)malloc(sizeof(RGB *) * h);

    if (NULL == img->vals) {
        return 1;
    }

    for (int i = 0; i < h; i++) {
        img->vals[i] = (RGB *)malloc(sizeof(RGB) * w);

        if (NULL == img->vals[i]) {
            return 1;
        }

        for (int j = 0; j < w; j++) {
            img->vals[i][j].r = 0;
            img->vals[i][j].g = 0;
            img->vals[i][j].b = 0;
        }
    }
    return 0;
}

void setPixelColor(RaymondImage *img, int x, int y, RGB color) {
    img->vals[y][x] = color;
}

#endif