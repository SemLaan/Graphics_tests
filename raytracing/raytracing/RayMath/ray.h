#pragma once

#include "vec3.h"

class Ray 
{
public:
	vec3 A;
	vec3 B;
	
	Ray() {}
	Ray(const vec3& a, const vec3& b) { A = a; B = b; }

	vec3 Origin() const { return A; }
	vec3 Direction() const { return B; }
	vec3 PointAtParameter(float t) const { return A + t * B; }
};