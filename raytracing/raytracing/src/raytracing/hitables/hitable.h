#pragma once

#include "raytracing/raymath/ray.h"

class Material;

struct HitRecord {
	float t = 0.f;
	Eigen::Vector3f p;
	Eigen::Vector3f normal;
	Material* matPtr = nullptr;

	HitRecord() = default;
};

class Hitable {
public:
	virtual bool Hit(const Ray& r, float t_min, float t_max, HitRecord& record) const = 0;
};