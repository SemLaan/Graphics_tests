#include "random.h"

namespace Utils
{
    float Random() {
        return ((float)rand()) / RAND_MAX;
    }

    glm::vec3 RandomInUnitDisk() {
        glm::vec3 p;
        do {
            p = 2.0f * glm::vec3(((float)rand()) / RAND_MAX, ((float)rand()) / RAND_MAX, 0) - glm::vec3(1, 1, 0);
        } while (glm::dot(p, p) >= 1.0);
        return p;
    }


    glm::vec3 RandomOnHorizontalUnitCircle() {
        glm::vec3 p;
        do {
            p = 2.0f * glm::vec3(((float)rand()) / RAND_MAX, 0, ((float)rand()) / RAND_MAX) - glm::vec3(1, 0, 1);
        } while (glm::dot(p, p) >= 1.0);
        return glm::normalize(p);
    }

    glm::vec3 RandomInUnitSphere() {
        glm::vec3 p;
        do {
            p = 2.0f * glm::vec3(((float)rand()) / RAND_MAX, ((float)rand()) / RAND_MAX, ((float)rand()) / RAND_MAX) - glm::vec3(1, 1, 1);
        } while (glm::length(p) >= 1.0);
        return p;
    }
}