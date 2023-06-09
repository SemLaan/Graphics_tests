#include "lambertian.h"

Lambertian::Lambertian(const glm::vec3& a)
	: albedo(a) 
{}

bool Lambertian::Scatter(const Ray& r_in, const HitRecord& record, glm::vec3& attenuation, Ray& scattered, uint32_t& seed) const {
	glm::vec3 target = record.p + record.normal + Utils::RandomInUnitSphere(seed);
	scattered = Ray(record.p, target - record.p);
	attenuation = albedo;
	return true;
}