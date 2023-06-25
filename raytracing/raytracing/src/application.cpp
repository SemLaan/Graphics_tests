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
    unsigned int maxThreads = std::thread::hardware_concurrency();
    unsigned int numThreads = 100;
    if (numThreads > maxThreads)
        numThreads = maxThreads;

    int nx = 600; // width of the image
    int ny = 300; // height of the image
    int ns = 200; // samples per pixel

    // Setup array for image data
    int numChannels = 4; // RGBA
    
    Camera cam;
    Hitable* scene = RandomScene(cam, nx, ny);

    unsigned char* imageData = new unsigned char[nx * ny * numChannels];
    std::thread** threadPool = new std::thread*[numThreads];
    
    std::cout << "starting with " << numThreads << " threads" << std::endl;
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    
    int nyStart = 0;
    for (unsigned int i = 0; i < numThreads; i++)
    {
        int rowsForThisThread = (int)std::floor(ny / (numThreads + 1));
        unsigned int excessRows = ny % (numThreads + 1);
        if (i < excessRows)
            rowsForThisThread++;
        int nyEnd = nyStart + rowsForThisThread;
        std::cout << "rows for this thread: " << nyEnd - nyStart << std::endl;
        threadPool[i] = new std::thread(Render, nx, nyStart, nyEnd, ny, ns, numChannels, &cam, scene, imageData, false);
        nyStart += rowsForThisThread;
    }

    std::cout << "rows for this thread: " << ny - nyStart << std::endl;
    Render(nx, nyStart, ny, ny, ns, numChannels, &cam, scene, imageData, true);

    for (unsigned int i = 0; i < numThreads; i++)
    {
        threadPool[i]->join();
    }

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[miliseconds]" << std::endl;
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::seconds> (end - begin).count() << "[seconds]" << std::endl;

    // write image to png file
    stbi_write_png("image.png", nx, ny, numChannels, imageData, nx * numChannels);
}
