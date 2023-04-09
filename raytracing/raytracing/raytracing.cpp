#include <iostream>
#include <fstream>
#include "cfloat"
#include <chrono>

#include "RayMath/hitablelist.h"
#include "RayMath/sphere.h"
#include "RayMath/camera.h"
#include "utils.h"
#include "RayMath/material.h"
#include "RayMath/metal.h"
#include "RayMath/lambertian.h"
#include "RayMath/dielectric.h"



vec3 Color(const Ray& r, Hitable* scene, int depth) {
    HitRecord record;
    if (scene->Hit(r, 0.001, FLT_MAX, record)) {
        Ray scattered;
        vec3 attenuation;
        if (depth < 50 && record.matPtr->Scatter(r, record, attenuation, scattered)) {
            return attenuation * Color(scattered, scene, depth + 1);
        }
        else {
            return vec3(0, 0, 0);
        }
    }
    else {
        vec3 unitDirection = UnitVector(r.Direction());
        float t = 0.5f * (unitDirection.y() + 1.0f);
        return (1.0f - t) * vec3(1.0f, 1.0f, 1.0f) + t * vec3(0.5f, 0.7f, 1.0f);
    }
}


int main()
{
    int nx = 200; // width of the image
    int ny = 100; // height of the image
    int ns = 100; // samples per pixel

    // Opening the image file and outputing the file settings into it
    std::ofstream imageFile;
    imageFile.open("Image.ppm");
    imageFile << "P3\n" << nx << " " << ny << "\n255\n";    

    // Defining the scene
    const int objectCount = 4;
    Hitable* list[objectCount];
    list[0] = new Sphere(vec3(0, 0, -1), 0.5, new Lambertian(vec3(0.8, 0.3, 0.3)));
    list[1] = new Sphere(vec3(0, -100.5, -1), 100, new Lambertian(vec3(0.8, 0.8, 0.0)));
    list[2] = new Sphere(vec3(1, 0, -1), 0.5, new Metal(vec3(0.8, 0.6, 0.2), 0.1));
    list[3] = new Sphere(vec3(-1, 0, -1), 0.5, new Dielectric(1.5));
    //list[4] = new Sphere(vec3(-1, 0, -1), -0.45, new Dielectric(1.5));

    Hitable* scene = new HitableList(list, objectCount);

    // Defining the camera
    vec3 lookFrom(3, 3, 2);
    vec3 lookAt(0, 0, -1);
    float distanceToFocus = (lookFrom - lookAt).Length();
    float aperture = 0.5;
    Camera cam(lookFrom, lookAt, vec3(0, 1, 0), 20, float(nx) / float(ny), aperture, distanceToFocus);
    
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    for (int j = ny - 1; j >= 0; j--)
    {
        if (j%10 == 0)
            std::cout << "progress: " << 1-(float)j/ny << std::endl;
        for (int i = 0; i < nx; i++)
        {
            vec3 color(0, 0, 0);
            for (int s = 0; s < ns; s++) {
                float u = float(i + (((double)rand()) / RAND_MAX)) / float(nx);
                float v = float(j + (((double)rand()) / RAND_MAX)) / float(ny);

                Ray r = cam.GetRay(u, v);

                color += Color(r, scene, 0);
            }
            
            color /= float(ns);
            color = vec3(sqrt(color[0]), sqrt(color[1]), sqrt(color[2]));
            int ir = int(255.99 * color[0]);
            int ig = int(255.99 * color[1]);
            int ib = int(255.99 * color[2]);
            imageFile << ir << " " << ig << " " << ib << "\n";
        }
    }

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[miliseconds]" << std::endl;
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::seconds> (end - begin).count() << "[seconds]" << std::endl;

    imageFile.close();
}
