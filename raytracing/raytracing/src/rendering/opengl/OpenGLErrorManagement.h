#pragma once

#ifdef DEBUG
#define ASSERT(x) if (!(x)) __debugbreak();
#define G(x) glClearError(); x; ASSERT(glLogCall(#x, __FILE__, __LINE__ ));
#define CUSTOMWARNING(x) std::cout << x << " "  << __FILE__  << " "  << __LINE__ << std::endl;
#else
#define ASSERT(x) x;
#define G(x) x;
#define CUSTOMWARNING(x) x;
#endif // DEBUG


namespace Renderer
{
	void glfw_error_callback(int error, const char* description);
	void glClearError();
	bool glLogCall(const char* function, const char* file, int line);
}