#pragma once
#include "ray.h"
#include "raytracing/hitables/hitablelist.h"
#include "raytracing/materials/material.h"


glm::vec3 TraceRay(const Ray& r, Hitable* scene, int depth);