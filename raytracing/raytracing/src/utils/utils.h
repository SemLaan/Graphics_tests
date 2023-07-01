#pragma once
#include "random.h"
#include "glm/glm.hpp"

namespace Utils
{
    float Schlick(float cosine, float ref_idx);
    bool Refract(const glm::vec3& v, const glm::vec3& n, float ni_over_nt, glm::vec3& refracted);
    glm::vec3 Reflect(const glm::vec3& v, const glm::vec3& n);
}