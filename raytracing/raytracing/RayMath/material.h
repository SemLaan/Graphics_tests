#pragma once
#include "ray.h"
#include "hitable.h"

class Material {
public:
	virtual bool Scatter(const Ray& r_in, const HitRecord& record, Eigen::Vector3f& attenuation, Ray& scattered) const = 0;
};