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
	Camera(glm::vec3 lookFrom, glm::vec3 lookAt, glm::vec3 vectorUp, float verticalFov, float aspect, float aperture, float focusDist);

	Ray GetRay(float s, float t);
};