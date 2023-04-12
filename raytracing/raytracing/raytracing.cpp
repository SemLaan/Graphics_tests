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


void Render(int nx, int ny, int ns, int numChannels, Camera* cam, Hitable* scene, unsigned int* imageData)
{
    for (int j = 0; j < ny; j++)
    {
        if (j % 10 == 0)
            std::cout << "progress: " << (float)j / ny << std::endl;
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
            imageData[index + 0] = ir; // R
            imageData[index + 1] = ig; // G
            imageData[index + 2] = ib; // B
            imageData[index + 3] = 255; // A
        }
    }
}


int main()
{
    int nx = 200; // width of the image
    int ny = 100; // height of the image
    int ns = 10; // samples per pixel

    // Setup array for image data
    int numChannels = 4; // RGBA
    
    Camera cam;
    Hitable* scene = SimpleScene(cam, nx, ny);

    
    std::cout << "starting" << std::endl;
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    unsigned int* imageData1 = new unsigned int[nx * ny * numChannels];
    std::thread worker(Render, nx, ny, ns, numChannels, &cam, scene, imageData1);
    worker.join();

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[miliseconds]" << std::endl;
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::seconds> (end - begin).count() << "[seconds]" << std::endl;

    unsigned char* imageData = new unsigned char[nx * ny * numChannels];

    for (int j = 0; j < ny; j++)
    {
        for (int i = 0; i < nx; i++)
        {
            int index = ((ny - j - 1) * nx + i) * numChannels;
            Eigen::Vector3f color(0, 0, 0);
            for (int s = 0; s < 1; s++) {
                color.x() += imageData1[index + 0];
                color.y() += imageData1[index + 1];
                color.z() += imageData1[index + 2];
            }
            color /= 1;
            // Write color data into array
            imageData[index + 0] = unsigned char(color.x()); // R
            imageData[index + 1] = unsigned char(color.y()); // G
            imageData[index + 2] = unsigned char(color.z()); // B
            imageData[index + 3] = unsigned char(255); // A
        }
    }

    // write image to png file
    stbi_write_png("image.png", nx, ny, numChannels, imageData, nx * numChannels);
}
