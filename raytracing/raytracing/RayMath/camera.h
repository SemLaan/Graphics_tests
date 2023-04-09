#pragma once

#include "../utils.h"
#include "ray.h"
#include <corecrt_math_defines.h>


class Camera {
public:
	vec3 lowerLeftCorner;
	vec3 horizontal;
	vec3 vertical;
	vec3 origin;
	vec3 u, v, w;
	float lensRadius;

	/// <summary>
	/// Makes a camera
	/// </summary>
	/// <param name="vfov">vertical fov in degrees</param>
	/// <param name="aspect">how many times the size of height is the width</param>
	Camera(vec3 lookFrom, vec3 lookAt, vec3 vectorUp, float verticalFov, float aspect, float aperture, float focusDist) {
		lensRadius = aperture / 2;

		// vertical fov in radians
		float theta = verticalFov * M_PI / 180;
		float halfHeight = tan(theta / 2);
		float halfWidth = aspect * halfHeight;

		origin = lookFrom;
		w = UnitVector(lookFrom - lookAt);
		u = UnitVector(Cross(vectorUp, w));
		v = Cross(w, u);

		lowerLeftCorner = origin - halfWidth * focusDist * u - halfHeight * focusDist * v - w * focusDist;
		horizontal = 2 * halfWidth * focusDist * u;
		vertical = 2 * halfHeight * focusDist * v;
	}

	Ray GetRay(float s, float t) {
		vec3 rd = lensRadius * RandomInUnitDisk();
		vec3 offset = u * rd.x() + v * rd.y();
		return Ray(origin + offset, lowerLeftCorner + s * horizontal + t * vertical - origin - offset);
	}
};