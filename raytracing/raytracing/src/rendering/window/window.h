#pragma once
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include "scene/scene.h"

namespace Renderer
{

	class Window
	{
	public:

		Window(Engine::Scene* scene, int width, int height, const char* windowTitle);
		~Window();

		void SetScene(Engine::Scene* scene);

		void StartGameLoop();

	private:
		GLFWwindow* m_window;
		Engine::Scene* m_scene;
		unsigned int m_screenWidth;
		unsigned int m_screenHeight;
	};
}