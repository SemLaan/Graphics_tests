#pragma once

#include "utils/utils.h"
#include "ray.h"
#include <corecrt_math_defines.h>


class Camera {
public:
	glm::vec3 lowerLeftCorner;
	glm::vec3 horizontal;
	glm::vec3 vertical;
	glm::vec3 origin;
	glm::vec3 u, v, w;
	float lensRadius = 0.f;

	Camera() = default;

	/// <summary>
	/// Makes a camera
	/// </summary>
	/// <param name="vfov">vertical fov in degrees</param>
	/// <param name="aspect">how many times the size of height is the width</param>
	Camera(glm::vec3 lookFrom, glm::vec3 lookAt, glm::vec3 vectorUp, float verticalFov, float aspect, float aperture, float focusDist) {
		lensRadius = aperture / 2;

		// vertical fov in radians
		float theta = verticalFov * (float)M_PI / 180;
		float halfHeight = tan(theta / 2);
		float halfWidth = aspect * halfHeight;

		origin = lookFrom;
		w = glm::normalize(lookFrom - lookAt);
		u = glm::normalize((glm::cross(vectorUp, w)));
		v = glm::cross(w, u);

		lowerLeftCorner = origin - halfWidth * focusDist * u - halfHeight * focusDist * v - w * focusDist;
		horizontal = 2 * halfWidth * focusDist * u;
		vertical = 2 * halfHeight * focusDist * v;
	}

	Ray GetRay(float s, float t) {
		glm::vec3 rd = lensRadius * RandomInUnitDisk();
		glm::vec3 offset = u * rd[0] + v * rd[1];
		return Ray(origin + offset, lowerLeftCorner + s * horizontal + t * vertical - origin - offset);
	}
};