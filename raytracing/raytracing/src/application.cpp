#include <stb/stb_image_write.h>

#include "raytracing/raytracinginclude.h"
#include "utils/utils.h"
#include "utils/scenes.h"
#include "rendering/window.h"

int i = 0;


void UpdateLoop()
{
    std::cout << i << std::endl;
    i++;
}


int main()
{

    int width = 600; // width of the image
    int height = 300; // height of the image
    int samples = 2; // samples per pixel

    Renderer::Window window = Renderer::Window(width, height, "beef");
    window.SetUpdateFunction(UpdateLoop);
    window.StartGameLoop();

    Camera cam;
    Hitable* scene = RandomScene(cam, width, height);

    unsigned char* imageData = RenderToArray(scene, cam, width, height, samples);

    // write image to png file
    stbi_write_png("image.png", width, height, TEXTURE_CHANNELS, imageData, width * TEXTURE_CHANNELS);

    delete scene;
    delete[] imageData;
}
