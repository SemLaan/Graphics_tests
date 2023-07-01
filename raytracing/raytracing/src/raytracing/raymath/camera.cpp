#include "camera.h"

Camera::Camera(glm::vec3 lookFrom, glm::vec3 lookAt, glm::vec3 vectorUp, float verticalFov, float aspect, float aperture, float focusDist) {
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

Ray Camera::GetRay(float s, float t, uint32_t& seed) {
	glm::vec3 rd = lensRadius * Utils::RandomInUnitDisk(seed);
	glm::vec3 offset = u * rd[0] + v * rd[1];
	return Ray(origin + offset, lowerLeftCorner + s * horizontal + t * vertical - origin - offset);
}