#pragma once
#include "material.h"

class BlackHole : public Material {
public:
	float schwarzschildRadius;
	float gravityRadius;
	float gravityConst;
	int stepAmount;
	float stepSize;

	BlackHole(float ssRad, float gravRad, int _stepAmount, float _stepSize, float _gravityConst) : schwarzschildRadius(ssRad), gravityRadius(gravRad), stepAmount(_stepAmount), stepSize(_stepSize), gravityConst(_gravityConst) {}
	virtual bool Scatter(const Ray& r_in, const HitRecord& record, Eigen::Vector3f& attenuation, Ray& scattered) const {
		Eigen::Vector3f currentRayPos = record.p;
		Eigen::Vector3f currentRayDir = r_in.Direction();
		Eigen::Vector3f holeCenter = record.p - (record.normal * gravityRadius);

		for (int i = 0; i < stepAmount; i++) {
			currentRayPos += currentRayDir * stepSize;

			Eigen::Vector3f dirToCentre = holeCenter - currentRayPos;
			float distanceFromCenter = dirToCentre.norm();
			dirToCentre /= distanceFromCenter;

			float force = gravityConst / (distanceFromCenter * distanceFromCenter);
			currentRayDir = (currentRayDir + dirToCentre * force).normalized();

			if (distanceFromCenter < schwarzschildRadius) {
				return false;
			}
			else if (distanceFromCenter > gravityRadius)
			{
				scattered = Ray(currentRayPos, currentRayDir);
				attenuation = Eigen::Vector3f(1.0, 1.0, 1.0);
				return true;
			}
		}
		return false;
	}
};