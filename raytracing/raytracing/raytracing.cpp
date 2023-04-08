#include <iostream>
#include <fstream>
#include "cfloat"
#include <chrono>

#include "RayMath/hitablelist.h"
#include "RayMath/sphere.h"


vec3 Color(const Ray& r, Hitable* scene) {
    HitRecord record;
    if (scene->Hit(r, 0.0, FLT_MAX, record)) {
        return 0.5 * vec3(record.normal.x() + 1, record.normal.y() + 1, record.normal.z() + 1);
    }
    else {
        vec3 unitDirection = UnitVector(r.Direction());
        float t = 0.5f * (unitDirection.y() + 1.0f);
        return (1.0f - t) * vec3(1.0f, 1.0f, 1.0f) + t * vec3(0.5f, 0.7f, 1.0f);
    }
}


int main()
{
    int nx = 200;
    int ny = 100;

    // Opening the image file and outputing the file settings into it
    std::ofstream imageFile;
    imageFile.open("Image.ppm");
    imageFile << "P3\n" << nx << " " << ny << "\n255\n";

    // Defining the camera?
    vec3 lowerLeftCorner(-2.0, -1.0, -1.0);
    vec3 horizontal(4.0, 0.0, 0.0);
    vec3 vertical(0.0, 2.0, 0.0);
    vec3 origin(0.0, 0.0, 0.0);

    // Defining the scene
    const int objectCount = 2;
    Hitable* list[objectCount];
    list[0] = new Sphere(vec3(0, 0, -1), 0.5);
    list[1] = new Sphere(vec3(0, -100.5, -1), 100);
    Hitable* scene = new HitableList(list, objectCount);

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    for (int j = ny - 1; j >= 0; j--)
    {
        for (int i = 0; i < nx; i++)
        {
            float u = float(i) / float(nx);
            float v = float(j) / float(ny);

            Ray r(origin, lowerLeftCorner + u * horizontal + v * vertical);

            vec3 color = Color(r, scene);
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
