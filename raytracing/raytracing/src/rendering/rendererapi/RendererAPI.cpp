#include "rendererapi.h"
#include "rendering/opengl/openglrendererapi.h"

namespace Renderer
{
	RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

	std::unique_ptr<RendererAPI> RendererAPI::Create()
	{
		return std::make_unique<OpenGLRendererAPI>();
	}
}