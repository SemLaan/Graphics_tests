#pragma once
#include "raytracing/raymath/ray.h"
#include "raytracing/hitables/hitable.h"

class Material {
public:
	virtual bool Scatter(const Ray& r_in, const HitRecord& record, Eigen::Vector3f& attenuation, Ray& scattered) const = 0;
};