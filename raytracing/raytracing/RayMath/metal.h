#pragma once
#include "material.h"
#include "../utils.h"

class Metal : public Material {
public:
	vec3 albedo;
	float fuzz;

	Metal(const vec3& a, float f) : albedo(a) { if (f < 1) fuzz = f; else fuzz = 1; }
	virtual bool Scatter(const Ray& r_in, const HitRecord& record, vec3& attenuation, Ray& scattered) const {
		vec3 reflected = Reflect(UnitVector(r_in.Direction()), record.normal);
		scattered = Ray(record.p, reflected + fuzz * RandomInUnitSphere());
		attenuation = albedo;
		return (Dot(scattered.Direction(), record.normal) > 0);
	}
};