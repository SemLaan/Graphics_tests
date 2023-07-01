#pragma once
#include "raytracing/raytracinginclude.h"
#include "utils/scenes.h"

#include <iostream>
#include <thread>



#define TEXTURE_CHANNELS 4U

namespace Raytracing
{

    void RenderSubset(int width, int startRow, int endRow, int height, int ns, Camera* cam, Hitable* scene, float* imageData, uint32_t seed, bool printProgress = false);

    void RenderToArray(float* imgData, Hitable* scene, Camera& cam, unsigned int width, unsigned int height, unsigned int ns, uint32_t seed, bool verbose = false);
};
