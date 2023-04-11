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

Hitable* RandomScene() {
    int n = 500;
    Hitable** list = new Hitable*[n + 1];
    list[0] = new Sphere(Eigen::Vector3f(0, -1000, 0), 1000, new Lambertian(Eigen::Vector3f(0.5, 0.5, 0.5)));
    int i = 1;
    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            float chooseMat = Random();
            if (chooseMat < 0.4)
                continue;
            Eigen::Vector3f center(a + 0.9 * Random(), 0.2, b + 0.9 * Random());
            if ((center - Eigen::Vector3f(4, 0.2, 0)).norm() > 0.9) {
                if (chooseMat < 0.8) { // diffuse
                    list[i++] = new Sphere(center, 0.2, 
                                new Lambertian(Eigen::Vector3f(Random() * Random(), Random() * Random(), Random() * Random())));
                }
                else if (chooseMat < 0.95) { // metal
                    list[i++] = new Sphere(center, 0.2,
                        new Metal(Eigen::Vector3f(0.5 * (1 + Random()), 0.5 * (1 + Random()), 0.5 * (1 + Random())), 0.5 * (1 + Random())));
                }
                else { // glass
                    list[i++] = new Sphere(center, 0.2, new Dielectric(1.5));
                }
            }
        }
    }
    //new Dielectric(1.5)
    list[i++] = new Sphere(Eigen::Vector3f(0, 1, 0), 1.0, new Metal(Eigen::Vector3f(0.7, 0.6, 0.5), 0));
    list[i++] = new Sphere(Eigen::Vector3f(-4, 1, 0), 1.0, new Lambertian(Eigen::Vector3f(0.4, 0.2, 0.1)));
    //list[i++] = new Sphere(Eigen::Vector3f(4, 1, 0), 1.0, new Metal(Eigen::Vector3f(0.7, 0.6, 0.5), 0));
    float blackHoleSize = 1.0f;
    list[i++] = new Sphere(Eigen::Vector3f(4, 1.5, 0), blackHoleSize, new BlackHole(0.09, blackHoleSize, 1000, 0.01, 0.0008));

    return new HitableList(list, i);
}


int main()
{
    int nx = 1000; // width of the image
    int ny = 500; // height of the image
    int ns = 50; // samples per pixel

    // Setup array for image data
    int numChannels = 4; // RGBA
    unsigned char* imageData = new unsigned char[nx * ny * numChannels];

    // Defining the scene
    const int objectCount = 4;
    Hitable* list[objectCount];
    list[0] = new Sphere(Eigen::Vector3f(0, 0.3, -1), 0.5, new BlackHole(0.00, 0.5, 1000, 0.01, 0.0012));
    list[1] = new Sphere(Eigen::Vector3f(0, -100.5, -1), 100, new Lambertian(Eigen::Vector3f(0.8, 0.8, 0.0)));
    list[2] = new Sphere(Eigen::Vector3f(1, 0, -1), 0.5, new Metal(Eigen::Vector3f(0.8, 0.6, 0.2), 0.1));
    list[3] = new Sphere(Eigen::Vector3f(-1, 0, -1), 0.5, new Dielectric(1.5));

    //Hitable* scene = new HitableList(list, objectCount);
    Hitable* scene = RandomScene();

    // Defining the camera
    Eigen::Vector3f lookFrom(8, 2, 2);
    //Eigen::Vector3f lookFrom(2, 2, 2);
    Eigen::Vector3f lookAt(0, 0, -1);
    float distanceToFocus = (lookFrom - lookAt).norm();
    float aperture = 0.0;
    Camera cam(lookFrom, lookAt, Eigen::Vector3f(0, 1, 0), 35, float(nx) / float(ny), aperture, distanceToFocus);
    
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
