#pragma once

#include "hitable.h"
#include "raytracing/materials/material.h"

class Sphere : public Hitable {
public:
    glm::vec3 center;
	float radius;
    Material* matPtr = nullptr;

    Sphere() = default;
	Sphere(glm::vec3 cen, float r, Material* mat) : center(cen), radius(r), matPtr(mat) {};
	virtual bool Hit(const Ray& r, float t_min, float t_max, HitRecord& record) const;
};
