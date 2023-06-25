#pragma once
#include "raytracing/raytracinginclude.h"


void Render(int nx, int nyStart, int nyEnd, int ny, int ns, int numChannels, Camera* cam, Hitable* scene, unsigned char* imageData, bool printProgress = false)
{
    for (int j = nyStart; j < nyEnd; j++)
    {
        if (printProgress && j % 2 == 0)
            std::cout << "progress: " << (float)(j - nyStart) / (nyEnd - nyStart) << std::endl;
        for (int i = 0; i < nx; i++)
        {
            Eigen::Vector3f color(0, 0, 0);
            for (int s = 0; s < ns; s++) {
                float u = float(i + (((double)rand()) / RAND_MAX)) / float(nx);
                float v = float(j + (((double)rand()) / RAND_MAX)) / float(ny);

                Ray r = cam->GetRay(u, v);

                color += TraceRay(r, scene, 0);
            }

            color /= float(ns);
            color = Eigen::Vector3f(sqrt(color[0]), sqrt(color[1]), sqrt(color[2]));
            int ir = int(255.99 * color[0]);
            int ig = int(255.99 * color[1]);
            int ib = int(255.99 * color[2]);
            // Write color data into array
            int index = ((ny - j - 1) * nx + i) * numChannels;
            imageData[index + 0] = unsigned char(ir); // R
            imageData[index + 1] = unsigned char(ig); // G
            imageData[index + 2] = unsigned char(ib); // B
            imageData[index + 3] = unsigned char(255); // A
        }
    }
}