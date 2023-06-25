#pragma once
#include "ray.h"
#include "raytracing/hitables/hitablelist.h"
#include "raytracing/materials/material.h"


Eigen::Vector3f TraceRay(const Ray& r, Hitable* scene, int depth) {
    HitRecord record;
    if (scene->Hit(r, 0.001f, FLT_MAX, record)) {
        Ray scattered;
        Eigen::Vector3f attenuation;
        if (depth < 50 && record.matPtr->Scatter(r, record, attenuation, scattered)) {
            return attenuation.cwiseProduct(TraceRay(scattered, scene, depth + 1));
        }
        else {
            return Eigen::Vector3f(0, 0, 0);
        }
    }
    else {
        Eigen::Vector3f unitDirection = r.Direction().normalized();
        float t = 0.5f * (unitDirection.y() + 1.0f);
        return (1.0f - t) * Eigen::Vector3f(1.0f, 1.0f, 1.0f) + t * Eigen::Vector3f(0.5f, 0.7f, 1.0f);
    }
}