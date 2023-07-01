#include "scenes.h"
#define RANDOM_SEED 30

Hitable* RandomScene2(Camera& cam, int nx, int ny) {
    uint32_t seed = RANDOM_SEED;
    int n = 50;
    std::vector<Hitable*>* list = new std::vector<Hitable*>();
    list->reserve(500);
    list->push_back(new Sphere(glm::vec3(0, -1004, 0), 1000, new Lambertian(glm::vec3(0.5, 0.5, 0.5))));
    for (int i = 0; i < n; i++) {
        float chooseMat = Utils::RandomFloat(seed);
        glm::vec3 random = Utils::RandomOnHorizontalUnitCircle(seed);
        random *= Utils::RandomFloat(seed) * 1 + 2.5f;
        if (chooseMat < 0.8f) { // diffuse
            list->push_back(new Sphere(random, 0.2f,
                new Lambertian(glm::vec3(Utils::RandomFloat(seed) * Utils::RandomFloat(seed), Utils::RandomFloat(seed) * Utils::RandomFloat(seed), Utils::RandomFloat(seed) * Utils::RandomFloat(seed)))));
        }
        else if (chooseMat < 0.95f) { // metal
            list->push_back(new Sphere(random, 0.2f,
                new Metal(glm::vec3(0.5f * (1 + Utils::RandomFloat(seed)), 0.5f * (1 + Utils::RandomFloat(seed)), 0.5f * (1 + Utils::RandomFloat(seed))), 0.5f * (1 + Utils::RandomFloat(seed)))));
        }
        else { // glass
            list->push_back(new Sphere(random, 0.2f, new Dielectric(1.5f)));
        }

    }
    float blackHoleSize = 2.f;
    list->push_back(new Sphere(glm::vec3(0, 0, 0), blackHoleSize, new BlackHole(0.09f, blackHoleSize, 1000, 0.01f, 0.0020f)));

    // Defining the camera
    glm::vec3 lookFrom(4, 2, 0);
    glm::vec3 lookAt(0, 0, 0);
    float distanceToFocus = glm::length(lookFrom - lookAt);
    float aperture = 0.0;
    cam = Camera(lookFrom, lookAt, glm::vec3(0, 1, 0), 90, float(nx) / float(ny), aperture, distanceToFocus);

    return new HitableList(list->data(), (int)list->size());
}

Hitable* RandomScene(Camera& cam, int nx, int ny) {
    uint32_t seed = RANDOM_SEED;
    int n = 500;
    Hitable** list = new Hitable * [n + 1];
    list[0] = new Sphere(glm::vec3(0, -1000, 0), 1000, new Lambertian(glm::vec3(0.5, 0.5, 0.5)));
    int i = 1;
    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            float chooseMat = Utils::RandomFloat(seed);
            if (chooseMat < 0.4f)
                continue;
            glm::vec3 center(a + 0.9f * Utils::RandomFloat(seed), 0.2f, b + 0.9f * Utils::RandomFloat(seed));
            if (glm::length(center - glm::vec3(4, 0.2f, 0)) > 0.9f) {
                if (chooseMat < 0.8f) { // diffuse
                    list[i++] = new Sphere(center, 0.2f,
                        new Lambertian(glm::vec3(Utils::RandomFloat(seed) * Utils::RandomFloat(seed), Utils::RandomFloat(seed) * Utils::RandomFloat(seed), Utils::RandomFloat(seed) * Utils::RandomFloat(seed))));
                }
                else if (chooseMat < 0.95f) { // metal
                    list[i++] = new Sphere(center, 0.2f,
                        new Metal(glm::vec3(0.5f * (1 + Utils::RandomFloat(seed)), 0.5f * (1 + Utils::RandomFloat(seed)), 0.5f * (1 + Utils::RandomFloat(seed))), 0.5f * (1 + Utils::RandomFloat(seed))));
                }
                else { // glass
                    list[i++] = new Sphere(center, 0.2f, new Dielectric(1.5));
                }
            }
        }
    }
    list[i++] = new Sphere(glm::vec3(0, 1, 0), 1.0f, new Metal(glm::vec3(0.7f, 0.6f, 0.5f), 0));
    list[i++] = new Sphere(glm::vec3(-4, 1, 0), 1.0f, new Lambertian(glm::vec3(0.4f, 0.2f, 0.1f)));
    float blackHoleSize = 1.0f;
    list[i++] = new Sphere(glm::vec3(4, 1.5f, 0), blackHoleSize, new BlackHole(0.09f, blackHoleSize, 1000, 0.01f, 0.0008f));

    // Defining the camera
    glm::vec3 lookFrom(8, 2, 2);
    glm::vec3 lookAt(0, 0, -1);
    float distanceToFocus = glm::length(lookFrom - lookAt);
    float aperture = 0.0f;
    cam = Camera(lookFrom, lookAt, glm::vec3(0, 1, 0), 35, float(nx) / float(ny), aperture, distanceToFocus);

    return new HitableList(list, i);
}

Hitable* SimpleScene(Camera& cam, int nx, int ny) {
    // Defining the scene
    const int objectCount = 4;
    Hitable** list = new Hitable * [objectCount];
    list[0] = new Sphere(glm::vec3(0, 0.3f, -1), 0.5f, new BlackHole(0.05f, 0.5f, 1000, 0.01f, 0.0012f));
    list[1] = new Sphere(glm::vec3(0, -100.5f, -1), 100, new Lambertian(glm::vec3(0.8f, 0.8f, 0.0f)));
    list[2] = new Sphere(glm::vec3(1, 0, -1), 0.5f, new Metal(glm::vec3(0.8f, 0.6f, 0.2f), 0.1f));
    list[3] = new Sphere(glm::vec3(-1, 0, -1), 0.5f, new Dielectric(1.5f));

    // Defining the camera
    glm::vec3 lookFrom(2, 2, 2);
    glm::vec3 lookAt(0, 0, -1);
    float distanceToFocus = glm::length(lookFrom - lookAt);
    float aperture = 0.0;
    cam = Camera(lookFrom, lookAt, glm::vec3(0, 1, 0), 35, float(nx) / float(ny), aperture, distanceToFocus);

    return new HitableList(list, objectCount - 1);
}
