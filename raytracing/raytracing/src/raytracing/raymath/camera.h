#pragma once

#include "utils/utils.h"
#include "ray.h"
#include <corecrt_math_defines.h>


class Camera {
public:
	Eigen::Vector3f lowerLeftCorner;
	Eigen::Vector3f horizontal;
	Eigen::Vector3f vertical;
	Eigen::Vector3f origin;
	Eigen::Vector3f u, v, w;
	float lensRadius;

	Camera() = default;

	/// <summary>
	/// Makes a camera
	/// </summary>
	/// <param name="vfov">vertical fov in degrees</param>
	/// <param name="aspect">how many times the size of height is the width</param>
	Camera(Eigen::Vector3f lookFrom, Eigen::Vector3f lookAt, Eigen::Vector3f vectorUp, float verticalFov, float aspect, float aperture, float focusDist) {
		lensRadius = aperture / 2;

		// vertical fov in radians
		float theta = verticalFov * (float)M_PI / 180;
		float halfHeight = tan(theta / 2);
		float halfWidth = aspect * halfHeight;

		origin = lookFrom;
		w = (lookFrom - lookAt).normalized();
		u = (vectorUp.cross(w)).normalized();
		v = w.cross(u);

		lowerLeftCorner = origin - halfWidth * focusDist * u - halfHeight * focusDist * v - w * focusDist;
		horizontal = 2 * halfWidth * focusDist * u;
		vertical = 2 * halfHeight * focusDist * v;
	}

	Ray GetRay(float s, float t) {
		Eigen::Vector3f rd = lensRadius * RandomInUnitDisk();
		Eigen::Vector3f offset = u * rd[0] + v * rd[1];
		return Ray(origin + offset, lowerLeftCorner + s * horizontal + t * vertical - origin - offset);
	}
};