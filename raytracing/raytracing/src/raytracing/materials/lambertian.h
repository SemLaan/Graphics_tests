#pragma once
#include "material.h"
#include "utils/utils.h"

class Lambertian : public Material {
public:
	glm::vec3 albedo;

	Lambertian(const glm::vec3& a) : albedo(a) {}
	virtual bool Scatter(const Ray& r_in, const HitRecord& record, glm::vec3& attenuation, Ray& scattered) const {
		glm::vec3 target = record.p + record.normal + RandomInUnitSphere();
		scattered = Ray(record.p, target - record.p);
		attenuation = albedo;
		return true;
	}
};