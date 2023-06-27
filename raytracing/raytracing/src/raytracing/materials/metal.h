#pragma once
#include "material.h"
#include "utils/utils.h"

class Metal : public Material {
public:
	glm::vec3 albedo;
	float fuzz;

	Metal(const glm::vec3& a, float f) : albedo(a) { if (f < 1) fuzz = f; else fuzz = 1; }
	virtual bool Scatter(const Ray& r_in, const HitRecord& record, glm::vec3& attenuation, Ray& scattered) const {
		glm::vec3 reflected = Reflect(glm::normalize(r_in.Direction()), record.normal);
		scattered = Ray(record.p, reflected + fuzz * RandomInUnitSphere());
		attenuation = albedo;
		return (glm::dot(scattered.Direction(), record.normal) > 0);
	}
};