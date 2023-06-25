#pragma once
#include <GLFW/glfw3.h>

namespace Renderer
{
	typedef void (*callback_function)();

	class Window
	{
	public:

		Window(int width, int height, const char* windowTitle);
		~Window();

		void SetUpdateFunction(callback_function func);

		void StartGameLoop();

	private:
		GLFWwindow* m_window;
		callback_function m_updateLoopFunc;
	};
}