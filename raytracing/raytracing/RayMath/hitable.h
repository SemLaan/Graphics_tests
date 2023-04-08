#pragma once

#include "ray.h"

struct HitRecord {
	float t;
	vec3 p;
	vec3 normal;
};

class Hitable {
public:
	virtual bool Hit(const Ray& r, float t_min, float t_max, HitRecord& record) const = 0;
};