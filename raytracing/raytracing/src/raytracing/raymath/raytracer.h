#pragma once
#include "ray.h"
#include "raytracing/hitables/hitablelist.h"
#include "raytracing/materials/material.h"


glm::vec3 TraceRay(const Ray& r, Hitable* scene, int depth) {
    HitRecord record;
    if (scene->Hit(r, 0.001f, FLT_MAX, record)) {
        Ray scattered;
        glm::vec3 attenuation;
        if (depth < 50 && record.matPtr->Scatter(r, record, attenuation, scattered)) {
            return attenuation * TraceRay(scattered, scene, depth + 1);
        }
        else {
            return glm::vec3(0, 0, 0);
        }
    }
    else {
        glm::vec3 unitDirection = glm::normalize(r.Direction());
        float t = 0.5f * (unitDirection.y + 1.0f);
        return (1.0f - t) * glm::vec3(1.0f, 1.0f, 1.0f) + t * glm::vec3(0.5f, 0.7f, 1.0f);
    }
}