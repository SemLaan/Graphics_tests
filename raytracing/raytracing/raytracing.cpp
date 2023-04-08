#include <iostream>
#include <fstream>
#include "RayMath/vec3.h"
#include "RayMath/ray.h"


float HitSphere(const vec3& center, float radius, const Ray& r) {
    // ABC formula to determine whether the given ray intersects the given sphere
    vec3 oc = r.Origin() - center;
    float a = Dot(r.Direction(), r.Direction());
    float b = 2.0 * Dot(oc, r.Direction());
    float c = Dot(oc, oc) - radius * radius;
    float discriminant = b * b - 4 * a * c;

    if (discriminant < 0) {
        return -1.0f;
    }
    else { // If there is a collision calculate how far away from the origin of the ray the collision is
        return (-b - sqrt(discriminant)) / (2.0 * a);
    }
}

vec3 Color(const Ray& r) {
    float t = HitSphere(vec3(0, 0, -1), 0.5f, r);
    if (t > 0.0) {
        vec3 N = UnitVector(r.PointAtParameter(t) - vec3(0, 0, -1));
        return 0.5 * vec3(N.x() + 1, N.y() + 1, N.z() + 1);
    }
    vec3 unitDirection = UnitVector(r.Direction());
    t = 0.5f * (unitDirection.y() + 1.0f);
    return (1.0f - t) * vec3(1.0f, 1.0f, 1.0f) + t * vec3(0.5f, 0.7f, 1.0f);
}


int main()
{
    int nx = 200;
    int ny = 100;

    // Opening the image file and outputing the file settings into it
    std::ofstream imageFile;
    imageFile.open("Image.ppm");
    imageFile << "P3\n" << nx << " " << ny << "\n255\n";

    // Defining the camera?
    vec3 lowerLeftCorner(-2.0, -1.0, -1.0);
    vec3 horizontal(4.0, 0.0, 0.0);
    vec3 vertical(0.0, 2.0, 0.0);
    vec3 origin(0.0, 0.0, 0.0);

    for (int j = ny - 1; j >= 0; j--)
    {
        for (int i = 0; i < nx; i++)
        {
            float u = float(i) / float(nx);
            float v = float(j) / float(ny);

            Ray r(origin, lowerLeftCorner + u * horizontal + v * vertical);

            vec3 color = Color(r);
            int ir = int(255.99 * color[0]);
            int ig = int(255.99 * color[1]);
            int ib = int(255.99 * color[2]);
            imageFile << ir << " " << ig << " " << ib << "\n";
        }
    }

    imageFile.close();
}
