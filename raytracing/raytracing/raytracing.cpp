#include <iostream>
#include <fstream>
#include "cfloat"
#include <chrono>
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



Eigen::Vector3f Color(const Ray& r, Hitable* scene, int depth) {
    HitRecord record;
    if (scene->Hit(r, 0.001, FLT_MAX, record)) {
        Ray scattered;
        Eigen::Vector3f attenuation;
        if (depth < 50 && record.matPtr->Scatter(r, record, attenuation, scattered)) {
            return attenuation.cwiseProduct(Color(scattered, scene, depth + 1));
        }
        else {
            return Eigen::Vector3f(0, 0, 0);
        }
    }
    else {
        Eigen::Vector3f unitDirection = r.Direction().normalized();
        float t = 0.5f * (unitDirection.y() + 1.0f);
        return (1.0f - t) * Eigen::Vector3f(1.0f, 1.0f, 1.0f) + t * Eigen::Vector3f(0.5f, 0.7f, 1.0f);
    }
}


int main()
{
    int nx = 200; // width of the image
    int ny = 100; // height of the image
    int ns = 10; // samples per pixel

    // Setup array for image data
    int numChannels = 4; // RGBA
    unsigned char* imageData = new unsigned char[nx * ny * numChannels];

    
    Camera cam;
    Hitable* scene = SimpleScene(cam, nx, ny);

    
    std::cout << "starting" << std::endl;
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    for (int j = 0; j < ny; j++)
    {
        if (j%10 == 0)
            std::cout << "progress: " << (float)j/ny << std::endl;
        for (int i = 0; i < nx; i++)
        {
            Eigen::Vector3f color(0, 0, 0);
            for (int s = 0; s < ns; s++) {
                float u = float(i + (((double)rand()) / RAND_MAX)) / float(nx);
                float v = float(j + (((double)rand()) / RAND_MAX)) / float(ny);

                Ray r = cam.GetRay(u, v);

                color += Color(r, scene, 0);
            }
            
            color /= float(ns);
            color = Eigen::Vector3f(sqrt(color[0]), sqrt(color[1]), sqrt(color[2]));
            int ir = int(255.99 * color[0]);
            int ig = int(255.99 * color[1]);
            int ib = int(255.99 * color[2]);
            // Write color data into array
            int index = ((ny-j-1) * nx + i) * numChannels;
            imageData[index + 0] = unsigned char(ir); // R
            imageData[index + 1] = unsigned char(ig); // G
            imageData[index + 2] = unsigned char(ib); // B
            imageData[index + 3] = unsigned char(255); // A
        }
    }

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[miliseconds]" << std::endl;
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::seconds> (end - begin).count() << "[seconds]" << std::endl;

    // write image to png file
    stbi_write_png("image.png", nx, ny, numChannels, imageData, nx * numChannels);
}
