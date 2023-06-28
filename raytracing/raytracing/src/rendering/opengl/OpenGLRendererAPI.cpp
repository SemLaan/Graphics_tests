#include "openglrendererapi.h"
#include "openglerrormanagement.h"

#include <glew/glew.h>

namespace Renderer
{
	// ------------------------------------------- ENUMS & CONVERSIONS -------------------------------------------------------
#pragma region ENUMS & CONVERSIONS
	static GLenum BlendOptionToOpenGLBlendOption(BlendOption blendOption)
	{
		switch (blendOption)
		{
		case BlendOption::ZERO:						return GL_ZERO;
		case BlendOption::ONE:							return GL_ONE;
		case BlendOption::SRC_COLOR:					return GL_SRC_COLOR;
		case BlendOption::ONE_MINUS_SRC_COLOR:			return GL_ONE_MINUS_SRC_COLOR;
		case BlendOption::DST_COLOR:					return GL_DST_COLOR;
		case BlendOption::ONE_MINUS_DST_COLOR:			return GL_ONE_MINUS_DST_COLOR;
		case BlendOption::SRC_ALPHA:					return GL_SRC_ALPHA;
		case BlendOption::ONE_MINUS_SRC_ALPHA:			return GL_ONE_MINUS_SRC_ALPHA;
		case BlendOption::DST_ALPHA:					return GL_DST_ALPHA;
		case BlendOption::ONE_MINUS_DST_ALPHA:			return GL_ONE_MINUS_DST_ALPHA;
		case BlendOption::CONSTANT_COLOR:				return GL_CONSTANT_COLOR;
		case BlendOption::ONE_MINUS_CONSTANT_COLOR:	return GL_ONE_MINUS_CONSTANT_COLOR;
		case BlendOption::CONSTANT_ALPHA:				return GL_CONSTANT_ALPHA;
		case BlendOption::ONE_MINUS_CONSTANT_ALPHA:	return GL_ONE_MINUS_CONSTANT_ALPHA;
		}
		return 0;
	}

	static GLenum StencilOptionToOpenGLStencilOption(StencilOption blendOption)
	{
		switch (blendOption)
		{
		case StencilOption::NEVER:		return GL_NEVER;
		case StencilOption::INCR:		return GL_INCR;
		case StencilOption::KEEP:		return GL_KEEP;
		case StencilOption::DECR:		return GL_DECR;
		case StencilOption::EQUAL:		return GL_EQUAL;
		case StencilOption::ALWAYS:		return GL_ALWAYS;
		}
		return 0;
	}
#pragma endregion
	
	
	// ------------------------------------------- RENDERER API -------------------------------------------------------
	void OpenGLRendererAPI::Init()
	{
		EnableBlend(true);
		SetBlendFunction(BlendOption::SRC_ALPHA, BlendOption::ONE_MINUS_SRC_ALPHA);
	}

	void OpenGLRendererAPI::Shutdown()
	{

	}

	void OpenGLRendererAPI::SetClearColor(float r, float g, float b, float a)
	{
		G(glClearColor(r, g, b, a));
	}

	void OpenGLRendererAPI::Clear()
	{
		G(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	}

	void OpenGLRendererAPI::ClearStencil()
	{
		G(glClear(GL_STENCIL_BUFFER_BIT));
	}

	void OpenGLRendererAPI::SetBlendFunction(BlendOption source, BlendOption destination)
	{
		G(glBlendFunc(BlendOptionToOpenGLBlendOption(source), BlendOptionToOpenGLBlendOption(destination)));
	}

	void OpenGLRendererAPI::SetStencilFunc(StencilOption func, int reference, unsigned int mask)
	{
		G(glStencilFunc(StencilOptionToOpenGLStencilOption(func), reference, mask));
	}

	void OpenGLRendererAPI::SetStencilMask(unsigned int mask)
	{
		G(glStencilMask(mask));
	}

	void OpenGLRendererAPI::SetStencilOp(StencilOption stencilFail, StencilOption zFail, StencilOption zPass)
	{
		G(glStencilOp(StencilOptionToOpenGLStencilOption(stencilFail), StencilOptionToOpenGLStencilOption(zFail), StencilOptionToOpenGLStencilOption(zPass)));
	}

	void OpenGLRendererAPI::EnableBlend(bool enable)
	{
		if (enable)
		{
			G(glEnable(GL_BLEND));
		}
		else
		{
			G(glDisable(GL_BLEND));
		}
	}

	void OpenGLRendererAPI::EnableDepthTest(bool enable)
	{
		if (enable)
		{
			G(glEnable(GL_DEPTH_TEST));
		}
		else
		{
			G(glDisable(GL_DEPTH_TEST));
		}
	}

	void OpenGLRendererAPI::EnableStencilTest(bool enable)
	{
		if (enable)
		{
			G(glEnable(GL_STENCIL_TEST));
		}
		else
		{
			G(glDisable(GL_STENCIL_TEST));
		}
	}

	void OpenGLRendererAPI::DrawIndexed(const std::shared_ptr<VertexArray> vertexArray, const std::shared_ptr<Shader> shader, unsigned int indexCount)
	{
		shader->Bind();
		vertexArray->Bind();
		vertexArray->GetIndexBuffer()->Bind();
		unsigned int count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
		G(glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr));
		drawCallCount++;
	}

	void OpenGLRendererAPI::DrawIndexedInstanced(const std::shared_ptr<VertexArray> vertexArray, const std::shared_ptr<Shader> shader, unsigned int instanceCount, unsigned int indexCount)
	{
		shader->Bind();
		vertexArray->Bind();
		vertexArray->GetIndexBuffer()->Bind();
		unsigned int count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
		G(glDrawElementsInstanced(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr, instanceCount));
		drawCallCount++;
	}
}