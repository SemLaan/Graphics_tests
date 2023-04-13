#include <iostream>
#include <fstream>
#include "cfloat"
#include <chrono>
#include <thread>
#include <stb/stb_image_write.h>

#include "RayMath/hitablelist.h"
#include "RayMath/sphere.h"
#include "RayMath/camera.h"
#include "utils.h"
#include "RayMath/material.h"
#include "RayMath/metal.h"
#include "RayMath/lambertian.h"
#include "RayMath/dielectric.h"
#include "RayMath/blackhole.h"
#include "scenes.h"
#include "RayMath/raytracer.h"


void Render(int nx, int nyStart, int nyEnd, int ny, int ns, int numChannels, Camera* cam, Hitable* scene, unsigned char* imageData, bool printProgress = false)
{
    for (int j = nyStart; j < nyEnd; j++)
    {
        if (printProgress && j % 2 == 0)
            std::cout << "progress: " << (float)(j - nyStart) / (nyEnd - nyStart) << std::endl;
        for (int i = 0; i < nx; i++)
        {
            Eigen::Vector3f color(0, 0, 0);
            for (int s = 0; s < ns; s++) {
                float u = float(i + (((double)rand()) / RAND_MAX)) / float(nx);
                float v = float(j + (((double)rand()) / RAND_MAX)) / float(ny);

                Ray r = cam->GetRay(u, v);

                color += TraceRay(r, scene, 0);
            }

            color /= float(ns);
            color = Eigen::Vector3f(sqrt(color[0]), sqrt(color[1]), sqrt(color[2]));
            int ir = int(255.99 * color[0]);
            int ig = int(255.99 * color[1]);
            int ib = int(255.99 * color[2]);
            // Write color data into array
            int index = ((ny - j - 1) * nx + i) * numChannels;
            imageData[index + 0] = unsigned char(ir); // R
            imageData[index + 1] = unsigned char(ig); // G
            imageData[index + 2] = unsigned char(ib); // B
            imageData[index + 3] = unsigned char(255); // A
        }
    }
}


int main()
{
    unsigned int maxThreads = std::thread::hardware_concurrency();
    unsigned int numThreads = 100;
    if (numThreads > maxThreads)
        numThreads = maxThreads;

    int nx = 400; // width of the image
    int ny = 200; // height of the image
    int ns = 100; // samples per pixel

    // Setup array for image data
    int numChannels = 4; // RGBA
    
    Camera cam;
    Hitable* scene = RandomScene(cam, nx, ny);

    unsigned char* imageData = new unsigned char[nx * ny * numChannels];
    std::thread** threadPool = new std::thread*[numThreads-1];
    
    std::cout << "starting with " << numThreads << " threads" << std::endl;
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    
    for (int i = 0; i < numThreads-1; i++)
    {
        int rowsPerThread = std::floor(ny / numThreads);
        int nyStart = rowsPerThread * i;
        int nyEnd = rowsPerThread * (i + 1);
        threadPool[i] = new std::thread(Render, nx, nyStart, nyEnd, ny, ns, numChannels, &cam, scene, imageData, false);
    }

    int rowsPerThread = std::floor(ny / numThreads);
    int nyStart = rowsPerThread * (numThreads - 1);
    Render(nx, nyStart, ny, ny, ns, numChannels, &cam, scene, imageData, true);

    for (int i = 0; i < numThreads - 1; i++)
    {
        threadPool[i]->join();
    }

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[miliseconds]" << std::endl;
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::seconds> (end - begin).count() << "[seconds]" << std::endl;

    // write image to png file
    stbi_write_png("image.png", nx, ny, numChannels, imageData, nx * numChannels);
}
