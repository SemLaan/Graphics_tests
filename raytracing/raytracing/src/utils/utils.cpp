#include "utils.h"

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

    float Schlick(float cosine, float ref_idx) {
        float r0 = (1 - ref_idx) / (1 + ref_idx);
        r0 = r0 * r0;
        return r0 + (1 - r0) * (float)pow((1 - cosine), 5);
    }

    bool Refract(const glm::vec3& v, const glm::vec3& n, float ni_over_nt, glm::vec3& refracted) {
        glm::vec3 uv = glm::normalize(v);
        float dt = glm::dot(uv, n);
        float discriminant = 1.0f - ni_over_nt * ni_over_nt * (1 - dt * dt);
        if (discriminant > 0) {
            refracted = ni_over_nt * (uv - n * dt) - n * sqrt(discriminant);
            return true;
        }
        else
            return false;
    }

    glm::vec3 Reflect(const glm::vec3& v, const glm::vec3& n) {
        return v - 2 * glm::dot(v, n) * n;
    }

    glm::vec3 RandomInUnitSphere() {
        glm::vec3 p;
        do {
            p = 2.0f * glm::vec3(((float)rand()) / RAND_MAX, ((float)rand()) / RAND_MAX, ((float)rand()) / RAND_MAX) - glm::vec3(1, 1, 1);
        } while (glm::length(p) >= 1.0);
        return p;
    }
}