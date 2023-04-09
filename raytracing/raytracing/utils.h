#pragma once
#include "RayMath/vec3.h"

vec3 RandomInUnitDisk() {
    vec3 p;
    do {
        p = 2.0 * vec3(((double)rand()) / RAND_MAX, ((double)rand()) / RAND_MAX, 0) - vec3(1, 1, 0);
    } while (Dot(p, p) >= 1.0);
    return p;
}

float Schlick(float cosine, float ref_idx) {
    float r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
}

bool Refract(const vec3& v, const vec3& n, float ni_over_nt, vec3& refracted) {
    vec3 uv = UnitVector(v);
    float dt = Dot(uv, n);
    float discriminant = 1.0 - ni_over_nt * ni_over_nt * (1 - dt * dt);
    if (discriminant > 0) {
        refracted = ni_over_nt * (uv - n * dt) - n * sqrt(discriminant);
        return true;
    }
    else
        return false;
}

vec3 Reflect(const vec3& v, const vec3& n) {
    return v - 2 * Dot(v, n) * n;
}

vec3 RandomInUnitSphere() {
    vec3 p;
    do {
        p = 2.0 * vec3(((double)rand()) / RAND_MAX, ((double)rand()) / RAND_MAX, ((double)rand()) / RAND_MAX) - vec3(1, 1, 1);
    } while (p.SquaredLength() >= 1.0);
    return p;
}