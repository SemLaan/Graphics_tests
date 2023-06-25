#pragma once
#include "material.h"
#include "utils/utils.h"

class Metal : public Material {
public:
	Eigen::Vector3f albedo;
	float fuzz;

	Metal(const Eigen::Vector3f& a, float f) : albedo(a) { if (f < 1) fuzz = f; else fuzz = 1; }
	virtual bool Scatter(const Ray& r_in, const HitRecord& record, Eigen::Vector3f& attenuation, Ray& scattered) const {
		Eigen::Vector3f reflected = Reflect(r_in.Direction().normalized(), record.normal);
		scattered = Ray(record.p, reflected + fuzz * RandomInUnitSphere());
		attenuation = albedo;
		return (scattered.Direction().dot(record.normal) > 0);
	}
};