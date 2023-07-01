#include "metal.h"


Metal::Metal(const glm::vec3& a, float f)
	: albedo(a) 
{ 
	if (f < 1) 
		fuzz = f; 
	else 
		fuzz = 1; 
}


bool Metal::Scatter(const Ray& r_in, const HitRecord& record, glm::vec3& attenuation, Ray& scattered, uint32_t& seed) const
{
	glm::vec3 reflected = Utils::Reflect(glm::normalize(r_in.Direction()), record.normal);
	scattered = Ray(record.p, reflected + fuzz * Utils::RandomInUnitSphere(seed));
	attenuation = albedo;
	return (glm::dot(scattered.Direction(), record.normal) > 0);
}