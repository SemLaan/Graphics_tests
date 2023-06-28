#include "window.h"
#include "rendering/rendererapi/rendercommand.h"
#include <iostream>

namespace Renderer
{
	Window::Window(Engine::Scene* scene, int width, int height, const char* windowTitle)
	{
        m_screenWidth = width;
        m_screenHeight = height;
        /* Initialize the library */
        glfwInit();

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


        /* Create a windowed mode window and its OpenGL context */
        m_window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);

        /* Make the window's context current */
        glfwMakeContextCurrent(m_window);

        glfwSwapInterval(1);

        std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
        std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

        glewInit();

        m_scene = scene;
        m_scene->Init(width, height);
        Renderer::RenderCommand::Init();
        Renderer::RenderCommand::SetClearColor(0.5f, 0.5f, 0.5f, 0);
	}

    Window::~Window()
    {
        Renderer::RenderCommand::Shutdown();
        m_scene->Shutdown();
        glfwTerminate();
    }

    void Window::StartGameLoop()
    {
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(m_window))
        {
            /* Render here */
            Renderer::RenderCommand::Clear();

            m_scene->Update();
            m_scene->Render();

            /* Swap front and back buffers */
            glfwSwapBuffers(m_window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }

    void Window::SetScene(Engine::Scene* scene)
    {
        m_scene->Shutdown();
        m_scene = scene;
        m_scene->Init(m_screenWidth, m_screenHeight);
    }
}