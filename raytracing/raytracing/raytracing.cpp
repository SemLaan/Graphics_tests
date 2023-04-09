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

Hitable* RandomScene() {
    int n = 500;
    Hitable** list = new Hitable * [n + 1];
    list[0] = new Sphere(vec3(0, -1000, 0), 1000, new Lambertian(vec3(0.5, 0.5, 0.5)));
    int i = 1;
    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            float chooseMat = Random();
            vec3 center(a + 0.9 * Random(), 0.2, b + 0.9 * Random());
            if ((center - vec3(4, 0.2, 0)).Length() > 0.9) {
                if (chooseMat < 0.8) { // diffuse
                    list[i++] = new Sphere(center, 0.2, 
                                new Lambertian(vec3(Random() * Random(), Random() * Random(), Random() * Random())));
                }
                else if (chooseMat < 0.95) { // metal
                    list[i++] = new Sphere(center, 0.2,
                        new Metal(vec3(0.5 * (1 + Random()), 0.5 * (1 + Random()), 0.5 * (1 + Random())), 0.5 * (1 + Random())));
                }
                else { // glass
                    list[i++] = new Sphere(center, 0.2, new Dielectric(1.5));
                }
            }
        }
    }

    list[i++] = new Sphere(vec3(0, 1, 0), 1.0, new Dielectric(1.5));
    list[i++] = new Sphere(vec3(-4, 1, 0), 1.0, new Lambertian(vec3(0.4, 0.2, 0.1)));
    list[i++] = new Sphere(vec3(4, 1, 0), 1.0, new Metal(vec3(0.7, 0.6, 0.5), 0));

    return new HitableList(list, i);
}


int main()
{
    int nx = 1000; // width of the image
    int ny = 500; // height of the image
    int ns = 100; // samples per pixel

    // Opening the image file and outputing the file settings into it
    std::ofstream imageFile;
    imageFile.open("Image.ppm");
    imageFile << "P3\n" << nx << " " << ny << "\n255\n";    

    // Defining the scene
    Hitable* scene = RandomScene();

    // Defining the camera
    vec3 lookFrom(8, 2, 2);
    vec3 lookAt(0, 0, -1);
    float distanceToFocus = (lookFrom - lookAt).Length();
    float aperture = 0.1;
    Camera cam(lookFrom, lookAt, vec3(0, 1, 0), 35, float(nx) / float(ny), aperture, distanceToFocus);
    
    std::cout << "starting" << std::endl;
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
