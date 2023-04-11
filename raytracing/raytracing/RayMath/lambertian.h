#pragma once
#include "material.h"
#include "../utils.h"

class Lambertian : public Material {
public:
	Eigen::Vector3f albedo;

	Lambertian(const Eigen::Vector3f& a) : albedo(a) {}
	virtual bool Scatter(const Ray& r_in, const HitRecord& record, Eigen::Vector3f& attenuation, Ray& scattered) const {
		Eigen::Vector3f target = record.p + record.normal + RandomInUnitSphere();
		scattered = Ray(record.p, target - record.p);
		attenuation = albedo;
		return true;
	}
};