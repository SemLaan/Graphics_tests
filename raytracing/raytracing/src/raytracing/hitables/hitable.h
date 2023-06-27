#pragma once

#include "raytracing/raymath/ray.h"

class Material;

struct HitRecord {
	float t = 0.f;
	glm::vec3 p;
	glm::vec3 normal;
	Material* matPtr = nullptr;

	HitRecord() = default;
};

class Hitable {
public:
	virtual bool Hit(const Ray& r, float t_min, float t_max, HitRecord& record) const = 0;
};