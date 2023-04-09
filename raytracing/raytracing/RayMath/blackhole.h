#pragma once
#include "material.h"
#include "vec3.h"

class BlackHole : public Material {
public:
	float schwarzschildRadius;
	float gravityRadius;
	float gravityConst;
	int stepAmount;
	float stepSize;

	BlackHole(float ssRad, float gravRad, int _stepAmount, float _stepSize, float _gravityConst) : schwarzschildRadius(ssRad), gravityRadius(gravRad), stepAmount(_stepAmount), stepSize(_stepSize), gravityConst(_gravityConst) {}
	virtual bool Scatter(const Ray& r_in, const HitRecord& record, vec3& attenuation, Ray& scattered) const {
		vec3 currentRayPos = record.p;
		vec3 currentRayDir = r_in.Direction();
		vec3 holeCenter = record.p - (record.normal * gravityRadius);

		for (int i = 0; i < stepAmount; i++) {
			currentRayPos += currentRayDir * stepSize;

			vec3 dirToCentre = holeCenter - currentRayPos;
			float distanceFromCenter = dirToCentre.Length();
			dirToCentre /= distanceFromCenter;

			float force = gravityConst / (distanceFromCenter * distanceFromCenter);
			currentRayDir = UnitVector(currentRayDir + dirToCentre * force);

			if (distanceFromCenter < schwarzschildRadius) {
				return false;
			}
			else if (distanceFromCenter > gravityRadius)
			{
				scattered = Ray(currentRayPos, currentRayDir);
				attenuation = vec3(1.0, 1.0, 1.0);
				return true;
			}
		}
		return false;
	}
};