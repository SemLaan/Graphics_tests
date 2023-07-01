#pragma once
#include "glm/glm.hpp"

namespace Utils
{
    uint32_t PCG_Hash(uint32_t input);
    float RandomFloat(uint32_t& seed);
    glm::vec3 RandomInUnitDisk(uint32_t& seed);
    glm::vec3 RandomOnHorizontalUnitCircle(uint32_t& seed);
    glm::vec3 RandomInUnitSphere(uint32_t& seed);
}