#pragma once
#include "material.h"
#include "utils/utils.h"

class Dielectric : public Material {
public:
	float ref_idx;

	Dielectric(float ri);
	
	bool Scatter(const Ray& r_in, const HitRecord& record, glm::vec3& attenuation, Ray& scattered) const override;
};