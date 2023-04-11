#pragma once
#include <Eigen/Dense>

class Ray 
{
public:
	Eigen::Vector3f A;
	Eigen::Vector3f B;
	
	Ray() {}
	Ray(const Eigen::Vector3f& a, const Eigen::Vector3f& b) { A = a; B = b; }

	Eigen::Vector3f Origin() const { return A; }
	Eigen::Vector3f Direction() const { return B; }
	Eigen::Vector3f PointAtParameter(float t) const { return A + t * B; }
};