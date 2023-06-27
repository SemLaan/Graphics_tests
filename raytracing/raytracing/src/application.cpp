#include <stb/stb_image_write.h>

#include "raytracing/raytracinginclude.h"
#include "utils/utils.h"
#include "utils/scenes.h"
#include "rendering/window.h"

int width = 600; // width of the image
int height = 300; // height of the image
int samples = 1; // samples per pixel

bool init = true;

Camera cam;
Hitable* scene;
unsigned int* imageData;
unsigned char* dividedImage;
int loopCounter = 0;

unsigned int vb;
unsigned int ib; 
unsigned int va;
unsigned int texture;


void UpdateLoop()
{
    if (init)
    {
        init = false;

        scene = RandomScene(cam, width, height);
        imageData = new unsigned int[width * height * TEXTURE_CHANNELS];
        dividedImage = new unsigned char[width * height * TEXTURE_CHANNELS];

        glGenBuffers(1, &vb);
    }

    RenderToArray(imageData, scene, cam, width, height, samples);

    for (unsigned int i = 0; i < width * height * TEXTURE_CHANNELS; i++)
    {
        dividedImage[i] = imageData[i] / (1 + loopCounter);
    }

    loopCounter++;
}


int main()
{

    Renderer::Window window = Renderer::Window(width, height, "beef");
    window.SetUpdateFunction(UpdateLoop);
    window.StartGameLoop();

    // write image to png file
    for (unsigned int i = 0; i < width * height * TEXTURE_CHANNELS; i++)
    {
        dividedImage[i] = imageData[i] / (1 + loopCounter);
    }

    stbi_write_png("image.png", width, height, TEXTURE_CHANNELS, dividedImage, width * TEXTURE_CHANNELS);

    delete scene;
    delete[] imageData;
}
