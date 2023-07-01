#include "rendering/window/window.h"
#include "scene/raytracingscene.h"


int main()
{
    int width = 1920; // width of the image
    int height = 1080; // height of the image

    Engine::Scene* raytracingScene = new Engine::RaytracingScene();
    Renderer::Window* window = new Renderer::Window(raytracingScene, width, height, "beef");
    
    window->StartGameLoop();

    delete window;
    delete raytracingScene;
}
