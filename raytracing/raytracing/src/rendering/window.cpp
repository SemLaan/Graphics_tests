#include "window.h"

namespace Renderer
{
	Window::Window(int width, int height, const char* windowTitle)
	{
        /* Initialize the library */
        glfwInit();

        /* Create a windowed mode window and its OpenGL context */
        m_window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);

        /* Make the window's context current */
        glfwMakeContextCurrent(m_window);
	}


    void Window::StartGameLoop()
    {
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(m_window))
        {
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);

            m_updateLoopFunc();

            /* Swap front and back buffers */
            glfwSwapBuffers(m_window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }


    Window::~Window()
    {
        glfwTerminate();
    }

    void Window::SetUpdateFunction(callback_function func)
    {
        m_updateLoopFunc = func;
    }
}