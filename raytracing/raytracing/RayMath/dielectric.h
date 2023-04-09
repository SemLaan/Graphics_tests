#pragma once
#include "material.h"
#include "../utils.h"

class Dielectric : public Material {
public:
	float ref_idx;
	Dielectric(float ri) : ref_idx(ri) {}
	
	virtual bool Scatter(const Ray& r_in, const HitRecord& record, vec3& attenuation, Ray& scattered) const {
		vec3 outwardNormal;
		vec3 reflected = Reflect(r_in.Direction(), record.normal);
		float ni_over_nt;
		attenuation = vec3(1.0, 1.0, 1.0);
		vec3 refracted;
		float reflectProb;
		float cosine;
		if (Dot(r_in.Direction(), record.normal) > 0) {
			outwardNormal = -record.normal;
			ni_over_nt = ref_idx;
			cosine = ref_idx * Dot(r_in.Direction(), record.normal) / r_in.Direction().Length();
		}
		else {
			outwardNormal = record.normal;
			ni_over_nt = 1.0 / ref_idx;
			cosine = -Dot(r_in.Direction(), record.normal) / r_in.Direction().Length();
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