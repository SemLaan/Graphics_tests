#pragma once
#include "material.h"
#include "utils/utils.h"

class Metal : public Material {
public:
	glm::vec3 albedo;
	float fuzz;

	Metal(const glm::vec3& a, float f);
	bool Scatter(const Ray& r_in, const HitRecord& record, glm::vec3& attenuation, Ray& scattered, uint32_t& seed) const override;
};