#pragma once
#include "material.h"

class BlackHole : public Material {
public:
	float schwarzschildRadius;
	float gravityRadius;
	float gravityConst;
	int stepAmount;
	float stepSize;

	BlackHole(float ssRad, float gravRad, int _stepAmount, float _stepSize, float _gravityConst);
	bool Scatter(const Ray& r_in, const HitRecord& record, glm::vec3& attenuation, Ray& scattered, uint32_t& seed) const override;
};