#pragma once
#include <glm/glm.hpp>

class Ray 
{
public:
	glm::vec3 A;
	glm::vec3 B;
	
	Ray() = default;
	Ray(const glm::vec3& a, const glm::vec3& b) : A(a), B(b) {}

	inline glm::vec3 Origin() const { return A; }
	inline glm::vec3 Direction() const { return B; }
	inline glm::vec3 PointAtParameter(float t) const { return A + t * B; }
};