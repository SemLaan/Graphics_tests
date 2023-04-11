#pragma once

#include "ray.h"

class Material;

struct HitRecord {
	float t;
	Eigen::Vector3f p;
	Eigen::Vector3f normal;
	Material* matPtr;
};

class Hitable {
public:
	virtual bool Hit(const Ray& r, float t_min, float t_max, HitRecord& record) const = 0;
};