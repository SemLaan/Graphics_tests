#pragma once

#include "ray.h"
#include <corecrt_math_defines.h>


class Camera {
public:
	vec3 lowerLeftCorner;
	vec3 horizontal;
	vec3 vertical;
	vec3 origin;

	/// <summary>
	/// Makes a camera
	/// </summary>
	/// <param name="vfov">vertical fov in degrees</param>
	/// <param name="aspect">how many times the size of height is the width</param>
	Camera(float verticalFov, float aspect) {
		// vertical fov in radians
		float theta = verticalFov * M_PI / 180;
		float halfHeight = tan(theta / 2);
		float halfWidth = aspect * halfHeight;

		lowerLeftCorner = vec3(-halfWidth, -halfHeight, -1.0);
		horizontal = vec3(2 * halfWidth, 0.0, 0.0);
		vertical = vec3(0.0, 2 * halfHeight, 0.0);
		origin = vec3(0.0, 0.0, 0.0);
	}

	Ray GetRay(float u, float v) {
		return Ray(origin, lowerLeftCorner + u * horizontal + v * vertical - origin);
	}
};