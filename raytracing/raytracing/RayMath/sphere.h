#pragma once

#include "hitable.h"
#include "material.h"

class Sphere : public Hitable {
public:
	vec3 center;
	float radius;
    Material* matPtr;

	Sphere() {}
	Sphere(vec3 cen, float r, Material* mat) : center(cen), radius(r), matPtr(mat) {};
	virtual bool Hit(const Ray& r, float t_min, float t_max, HitRecord& record) const;
};

bool Sphere::Hit(const Ray& r, float t_min, float t_max, HitRecord& record) const {
    // ABC formula to determine whether the given ray intersects the given sphere
    vec3 oc = r.Origin() - center;
    float a = Dot(r.Direction(), r.Direction());
    float b = Dot(oc, r.Direction());
    float c = Dot(oc, oc) - radius * radius;
    float discriminant = b * b - a * c;

    // If there is a collision 
    // calculate how far away from the origin of the ray the collision is
    if (discriminant > 0) {
        float temp = (-b - sqrt(discriminant)) / a;
        if (temp > t_min && temp < t_max) {
            record.t = temp;
            record.p = r.PointAtParameter(record.t);
            record.normal = (record.p - center) / radius;
            record.matPtr = matPtr;
            return true;
        }
        temp = (-b + sqrt(discriminant)) / a;
        if (temp > t_min && temp < t_max) {
            record.t = temp;
            record.p = r.PointAtParameter(record.t);
            record.normal = (record.p - center) / radius;
            record.matPtr = matPtr;
            return true;
        }
    }
    return false;
}