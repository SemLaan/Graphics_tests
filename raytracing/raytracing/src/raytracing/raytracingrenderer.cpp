#include "raytracingrenderer.h"

namespace Raytracing
{

    void RenderSubset(int width, int startRow, int endRow, int height, int ns, Camera* cam, Hitable* scene, unsigned int* imageData, bool printProgress)
    {
        for (int j = startRow; j < endRow; j++)
        {
            if (printProgress && j % 2 == 0)
                std::cout << "progress: " << (float)(j - startRow) / (endRow - startRow) << std::endl;
            for (int i = 0; i < width; i++)
            {
                glm::vec3 color(0, 0, 0);
                for (int s = 0; s < ns; s++) {
                    float u = float(i + (((double)rand()) / RAND_MAX)) / float(width);
                    float v = float(j + (((double)rand()) / RAND_MAX)) / float(height);

                    Ray r = cam->GetRay(u, v);

                    color += TraceRay(r, scene, 0);
                }

                color /= float(ns);
                color = glm::vec3(sqrt(color[0]), sqrt(color[1]), sqrt(color[2]));
                int ir = int(255.99 * color[0]);
                int ig = int(255.99 * color[1]);
                int ib = int(255.99 * color[2]);
                // Write color data into array
                int index = ((height - j - 1) * width + i) * TEXTURE_CHANNELS;
                imageData[index + 0] += unsigned char(ir); // R
                imageData[index + 1] += unsigned char(ig); // G
                imageData[index + 2] += unsigned char(ib); // B
                imageData[index + 3] += unsigned char(255); // A
            }
        }
    }


    void RenderToArray(unsigned int* imgData, Hitable* scene, Camera& cam, unsigned int width, unsigned int height, unsigned int ns)
    {
        unsigned int maxThreads = std::thread::hardware_concurrency();
        unsigned int numThreads = 100;
        if (numThreads > maxThreads)
            numThreads = maxThreads;

        std::thread** threadPool = new std::thread * [numThreads];

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
            std::cout << "rows for this thread: " << endRow - startRow << std::endl;
            threadPool[i] = new std::thread(RenderSubset, width, startRow, endRow, height, ns, &cam, scene, imgData, false);
            startRow += rowsForThisThread;
        }

        std::cout << "rows for this thread: " << height - startRow << std::endl;
        RenderSubset(width, startRow, height, height, ns, &cam, scene, imgData, true);

        for (unsigned int i = 0; i < numThreads; i++)
        {
            threadPool[i]->join();
        }

        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[miliseconds]" << std::endl;
        std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::seconds> (end - begin).count() << "[seconds]" << std::endl;

        for (unsigned int i = 0; i < numThreads; i++)
        {
            delete threadPool[i];
        }
        delete[] threadPool;
    }

}