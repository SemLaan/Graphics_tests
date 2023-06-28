#include "dielectric.h"

Dielectric::Dielectric(float ri) 
	: ref_idx(ri) 
{}

bool Dielectric::Scatter(const Ray& r_in, const HitRecord& record, glm::vec3& attenuation, Ray& scattered) const
{
	glm::vec3 outwardNormal;
	glm::vec3 reflected = Utils::Reflect(r_in.Direction(), record.normal);
	float ni_over_nt;
	attenuation = glm::vec3(1.0, 1.0, 1.0);
	glm::vec3 refracted;
	float reflectProb;
	float cosine;
	if (glm::dot(r_in.Direction(), record.normal) > 0) {
		outwardNormal = -record.normal;
		ni_over_nt = ref_idx;
		cosine = ref_idx * glm::dot(r_in.Direction(), record.normal) / glm::length(r_in.Direction());
	}
	else {
		outwardNormal = record.normal;
		ni_over_nt = 1.0f / ref_idx;
		cosine = -glm::dot(r_in.Direction(), record.normal) / glm::length(r_in.Direction());
	}
	if (Utils::Refract(r_in.Direction(), outwardNormal, ni_over_nt, refracted)) {
		reflectProb = Utils::Schlick(cosine, ref_idx);
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