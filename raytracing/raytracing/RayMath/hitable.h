#pragma once

#include "ray.h"

class Material;

struct HitRecord {
	float t;
	vec3 p;
	vec3 normal;
	Material* matPtr;
};

class Hitable {
public:
	virtual bool Hit(const Ray& r, float t_min, float t_max, HitRecord& record) const = 0;
};