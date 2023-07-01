#include "random.h"

namespace Utils
{
    uint32_t PCG_Hash(uint32_t input)
    {
        uint32_t state = input * 747796405u + 2891336453u;
        uint32_t word = ((state >> ((state >> 28u) + 4u)) ^ state) * 277803737u;
        return (word >> 22u) ^ word;
    }

    float RandomFloat(uint32_t& seed) {
        seed = PCG_Hash(seed);
        return (float)seed / (float)std::numeric_limits<uint32_t>::max();
    }

    glm::vec3 RandomInUnitDisk(uint32_t& seed) {
        glm::vec3 p;
        do {
            p = 2.0f * glm::vec3(RandomFloat(seed), RandomFloat(seed), 0) - glm::vec3(1, 1, 0);
        } while (glm::dot(p, p) >= 1.0);
        return p;
    }


    glm::vec3 RandomOnHorizontalUnitCircle(uint32_t& seed) {
        glm::vec3 p;
        do {
            p = 2.0f * glm::vec3(RandomFloat(seed), 0, RandomFloat(seed)) - glm::vec3(1, 0, 1);
        } while (glm::dot(p, p) >= 1.0);
        return glm::normalize(p);
    }

    glm::vec3 RandomInUnitSphere(uint32_t& seed) {
        glm::vec3 p;
        do {
            p = 2.0f * glm::vec3(RandomFloat(seed), RandomFloat(seed), RandomFloat(seed)) - glm::vec3(1, 1, 1);
        } while (glm::length(p) >= 1.0);
        return p;
    }
}