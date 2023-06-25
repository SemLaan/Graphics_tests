#include <iostream>
#include <fstream>
#include "cfloat"
#include <chrono>
#include <thread>
#include <stb/stb_image_write.h>

#include "raytracing/raytracinginclude.h"
#include "utils/utils.h"
#include "utils/scenes.h"


int main()
{
    int width = 600; // width of the image
    int height = 300; // height of the image
    int samples = 2; // samples per pixel

    Camera cam;
    Hitable* scene = RandomScene(cam, width, height);

    unsigned char* imageData = RenderToArray(scene, cam, width, height, samples);

    // write image to png file
    stbi_write_png("image.png", width, height, TEXTURE_CHANNELS, imageData, width * TEXTURE_CHANNELS);

    delete scene;
    delete[] imageData;
}
