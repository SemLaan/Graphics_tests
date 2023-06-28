#pragma once
#include "raytracing/raytracinginclude.h"
#include "utils.h"
#include <vector>

Hitable* RandomScene2(Camera& cam, int nx, int ny);

Hitable* RandomScene(Camera& cam, int nx, int ny);

Hitable* SimpleScene(Camera& cam, int nx, int ny);