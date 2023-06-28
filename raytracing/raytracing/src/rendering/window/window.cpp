#include "window.h"

namespace Renderer
{
	Window::Window(Engine::Scene* scene, int width, int height, const char* windowTitle)
	{
        m_screenWidth = width;
        m_screenHeight = height;
        /* Initialize the library */
        glfwInit();

        /* Create a windowed mode window and its OpenGL context */
        m_window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);

        /* Make the window's context current */
        glfwMakeContextCurrent(m_window);

        glewInit();

        m_scene = scene;
        m_scene->Init(width, height);
	}

    Window::~Window()
    {
        m_scene->Shutdown();
        glfwTerminate();
    }

    void Window::StartGameLoop()
    {
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(m_window))
        {
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);

            m_scene->Update();

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