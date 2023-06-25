#pragma once
#include "raytracing/raytracinginclude.h"
#include "utils.h"
#include <vector>

Hitable* RandomScene2(Camera& cam, int nx, int ny) {
    int n = 50;
    std::vector<Hitable*>* list = new std::vector<Hitable*>();
    list->reserve(500);
    list->push_back(new Sphere(Eigen::Vector3f(0, -1004, 0), 1000, new Lambertian(Eigen::Vector3f(0.5, 0.5, 0.5))));
    for (int i = 0; i < n; i++) {
        float chooseMat = Random();
        Eigen::Vector3f random = RandomOnHorizontalUnitCircle();
        random *= Random() * 1 + 2.5f;
        if (chooseMat < 0.8f) { // diffuse
            list->push_back(new Sphere(random, 0.2f,
                new Lambertian(Eigen::Vector3f(Random() * Random(), Random() * Random(), Random() * Random()))));
        }
        else if (chooseMat < 0.95f) { // metal
            list->push_back(new Sphere(random, 0.2f,
                new Metal(Eigen::Vector3f(0.5f * (1 + Random()), 0.5f * (1 + Random()), 0.5f * (1 + Random())), 0.5f * (1 + Random()))));
        }
        else { // glass
            list->push_back(new Sphere(random, 0.2f, new Dielectric(1.5f)));
        }

    }
    float blackHoleSize = 2.f;
    list->push_back(new Sphere(Eigen::Vector3f(0, 0, 0), blackHoleSize, new BlackHole(0.09f, blackHoleSize, 1000, 0.01f, 0.0020f)));

    // Defining the camera
    Eigen::Vector3f lookFrom(4, 2, 0);
    Eigen::Vector3f lookAt(0, 0, 0);
    float distanceToFocus = (lookFrom - lookAt).norm();
    float aperture = 0.0;
    cam = Camera(lookFrom, lookAt, Eigen::Vector3f(0, 1, 0), 90, float(nx) / float(ny), aperture, distanceToFocus);

    return new HitableList(list->data(), (int)list->size());
}

Hitable* RandomScene(Camera& cam, int nx, int ny) {
    int n = 500;
    Hitable** list = new Hitable * [n + 1];
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
                        new Metal(Eigen::Vector3f(0.5 * ((double)1 + Random()), 0.5 * ((double)1 + Random()), 0.5 * ((double)1 + Random())), 0.5 * ((double)1 + Random())));
                }
                else { // glass
                    list[i++] = new Sphere(center, 0.2, new Dielectric(1.5));
                }
            }
        }
    }
    list[i++] = new Sphere(Eigen::Vector3f(0, 1, 0), 1.0, new Metal(Eigen::Vector3f(0.7, 0.6, 0.5), 0));
    list[i++] = new Sphere(Eigen::Vector3f(-4, 1, 0), 1.0, new Lambertian(Eigen::Vector3f(0.4, 0.2, 0.1)));
    float blackHoleSize = 2.f;
    list[i++] = new Sphere(Eigen::Vector3f(1, 5, 0), blackHoleSize, new BlackHole(0.09, blackHoleSize, 1000, 0.01, 0.0008));

    // Defining the camera
    Eigen::Vector3f lookFrom(2, 8, 2);
    Eigen::Vector3f lookAt(0, 0, -1);
    float distanceToFocus = (lookFrom - lookAt).norm();
    float aperture = 0.0;
    cam = Camera(lookFrom, lookAt, Eigen::Vector3f(0, 1, 0), 35, float(nx) / float(ny), aperture, distanceToFocus);

    return new HitableList(list, i);
}

Hitable* SimpleScene(Camera& cam, int nx, int ny) {
    // Defining the scene
    const int objectCount = 4;
    Hitable** list = new Hitable*[objectCount];
    list[0] = new Sphere(Eigen::Vector3f(0, 0.3f, -1), 0.5f, new BlackHole(0.05f, 0.5f, 1000, 0.01f, 0.0012f));
    list[1] = new Sphere(Eigen::Vector3f(0, -100.5f, -1), 100, new Lambertian(Eigen::Vector3f(0.8f, 0.8f, 0.0f)));
    list[2] = new Sphere(Eigen::Vector3f(1, 0, -1), 0.5f, new Metal(Eigen::Vector3f(0.8f, 0.6f, 0.2f), 0.1f));
    list[3] = new Sphere(Eigen::Vector3f(-1, 0, -1), 0.5f, new Dielectric(1.5f));

    // Defining the camera
    Eigen::Vector3f lookFrom(2, 2, 2);
    Eigen::Vector3f lookAt(0, 0, -1);
    float distanceToFocus = (lookFrom - lookAt).norm();
    float aperture = 0.0;
    cam = Camera(lookFrom, lookAt, Eigen::Vector3f(0, 1, 0), 35, float(nx) / float(ny), aperture, distanceToFocus);

    return new HitableList(list, objectCount-1);
}
