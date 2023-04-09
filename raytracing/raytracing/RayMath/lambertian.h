#pragma once
#include "material.h"
#include "../utils.h"

class Lambertian : public Material {
public:
	vec3 albedo;

	Lambertian(const vec3& a) : albedo(a) {}
	virtual bool Scatter(const Ray& r_in, const HitRecord& record, vec3& attenuation, Ray& scattered) const {
		vec3 target = record.p + record.normal + RandomInUnitSphere();
		scattered = Ray(record.p, target - record.p);
		attenuation = albedo;
		return true;
	}
};