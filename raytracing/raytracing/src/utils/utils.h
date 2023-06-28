#pragma once
#include "glm/glm.hpp"

namespace Utils
{

    float Random();
    glm::vec3 RandomInUnitDisk();
    glm::vec3 RandomOnHorizontalUnitCircle();
    glm::vec3 RandomInUnitSphere();

    float Schlick(float cosine, float ref_idx);
    bool Refract(const glm::vec3& v, const glm::vec3& n, float ni_over_nt, glm::vec3& refracted);
    glm::vec3 Reflect(const glm::vec3& v, const glm::vec3& n);
}