#include <iostream>
#include <fstream>
#include "vec3.h"


int main()
{
    int nx = 200;
    int ny = 100;

    std::ofstream imageFile;
    imageFile.open("Image.ppm");

    imageFile << "P3\n" << nx << " " << ny << "\n255\n";

    for (int j = ny - 1; j >= 0; j--)
    {
        for (int i = 0; i < nx; i++)
        {
            vec3 color(float(i) / float(nx), float(j) / float(ny), 0.2f);
            int ir = int(255.99 * color[0]);
            int ig = int(255.99 * color[1]);
            int ib = int(255.99 * color[2]);
            imageFile << ir << " " << ig << " " << ib << "\n";
        }
    }

    imageFile.close();
}
