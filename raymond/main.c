#include <stdlib.h>
#include <stdio.h>
#include "img.h"
#include "types.h"
#include "camera.h"
#include "ray.h"

int main() {
    int width = 100;
    int height = 100;
    
    point3 light = {100.0, 100.0, 0.0};

    RaymondImage img;
    if (initRaymondImg(&img, width, height)) {
        fputs("Dang the raymond image did not work", stderr);
    }

    RaymondCamera cam;
    if (initRaymondCam(&cam)) {
        fputs("Dang the raymond cam did not work", stderr);
    }

    sphere object;
    initSphere(&object, 50.0, 50.0, 50.0, 25.0);

    sphere objects[1] = {object};



    RaymondScene scene;
    initRaymondScene(&scene);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            ray raymond = fromCameraTo(&cam, x, y, width, height);

            RGB color = traceRay(raymond, scene);
            
            setPixelColor(&img, x, y, color);
        }
    }



    return 0;
}