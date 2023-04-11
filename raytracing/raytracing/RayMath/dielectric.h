#pragma once
#include "material.h"
#include "../utils.h"

class Dielectric : public Material {
public:
	float ref_idx;
	Dielectric(float ri) : ref_idx(ri) {}
	
	virtual bool Scatter(const Ray& r_in, const HitRecord& record, Eigen::Vector3f& attenuation, Ray& scattered) const {
		Eigen::Vector3f outwardNormal;
		Eigen::Vector3f reflected = Reflect(r_in.Direction(), record.normal);
		float ni_over_nt;
		attenuation = Eigen::Vector3f(1.0, 1.0, 1.0);
		Eigen::Vector3f refracted;
		float reflectProb;
		float cosine;
		if (r_in.Direction().dot(record.normal) > 0) {
			outwardNormal = -record.normal;
			ni_over_nt = ref_idx;
			cosine = ref_idx * r_in.Direction().dot(record.normal) / r_in.Direction().norm();
		}
		else {
			outwardNormal = record.normal;
			ni_over_nt = 1.0f / ref_idx;
			cosine = -r_in.Direction().dot(record.normal) / r_in.Direction().norm();
		}
		if (Refract(r_in.Direction(), outwardNormal, ni_over_nt, refracted)) {
			reflectProb = Schlick(cosine, ref_idx);
		}
		else {
			reflectProb = 1.0;
		}
		if (((double)rand()) / RAND_MAX < reflectProb) {
			scattered = Ray(record.p, reflected);
		}
		else {
			scattered = Ray(record.p, refracted);
		}
		return true;
	}
};