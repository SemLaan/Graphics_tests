#include "raytracingrenderer.h"

namespace Raytracing
{

    void RenderSubset(int width, int startRow, int endRow, int height, int ns, Camera* cam, Hitable* scene, float* imageData, uint32_t seed, bool printProgress)
    {
        for (int j = startRow; j < endRow; j++)
        {
            if (printProgress && j % 2 == 0)
                std::cout << "progress: " << (float)(j - startRow) / (endRow - startRow) << std::endl;
            for (int i = 0; i < width; i++)
            {
                glm::vec3 color(0, 0, 0);
                for (int s = 0; s < ns; s++) {
                    float u = float(i + Utils::RandomFloat(seed)) / float(width);
                    float v = float(j + Utils::RandomFloat(seed)) / float(height);

                    Ray r = cam->GetRay(u, v, seed);

                    color += TraceRay(r, scene, 0, seed);
                }

                color /= float(ns);
                // Write color data into array
                unsigned int index = ((height - j - 1) * width + i) * TEXTURE_CHANNELS;
                imageData[index + 0] += color.x; // R
                imageData[index + 1] += color.y; // G
                imageData[index + 2] += color.z; // B
                imageData[index + 3] += 1.f;

                /*unsigned int ir = unsigned int(255.99 * color[0]);
                unsigned int ig = unsigned int(255.99 * color[1]);
                unsigned int ib = unsigned int(255.99 * color[2]);
                // Write color data into array
                unsigned int index = ((height - j - 1) * width + i) * TEXTURE_CHANNELS;
                imageData[index + 0] += ir; // R
                imageData[index + 1] += ig; // G
                imageData[index + 2] += ib; // B
                imageData[index + 3] += 255; // A*/
            }
        }
    }


    void RenderToArray(float* imgData, Hitable* scene, Camera& cam, unsigned int width, unsigned int height, unsigned int ns, uint32_t seed, bool verbose)
    {
        unsigned int maxThreads = std::thread::hardware_concurrency();
        unsigned int numThreads = 500;
        if (numThreads > maxThreads)
            numThreads = maxThreads;

        std::thread** threadPool = new std::thread * [numThreads];

        if (verbose)
            std::cout << "starting with " << numThreads << " threads" << std::endl;
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

        int startRow = 0;
        for (unsigned int i = 0; i < numThreads; i++)
        {
            int rowsForThisThread = (int)std::floor(height / (numThreads + 1));
            unsigned int excessRows = height % (numThreads + 1);
            if (i < excessRows)
                rowsForThisThread++;
            int endRow = startRow + rowsForThisThread;
            if (verbose)
                std::cout << "rows for this thread: " << endRow - startRow << std::endl;
            threadPool[i] = new std::thread(RenderSubset, width, startRow, endRow, height, ns, &cam, scene, imgData, seed + i, false);
            startRow += rowsForThisThread;
        }

        if (verbose)
            std::cout << "rows for this thread: " << height - startRow << std::endl;
        RenderSubset(width, startRow, height, height, ns, &cam, scene, imgData, seed + 10000, false);

        for (unsigned int i = 0; i < numThreads; i++)
        {
            threadPool[i]->join();
        }

        if (verbose) 
        {
            std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
            std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[miliseconds]" << std::endl;
            std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::seconds> (end - begin).count() << "[seconds]" << std::endl;
        }

        for (unsigned int i = 0; i < numThreads; i++)
        {
            delete threadPool[i];
        }
        delete[] threadPool;
    }

}