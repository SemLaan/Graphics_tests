#pragma once
#include "rendering/rendererapi/RendererAPI.h"

namespace Renderer
{
	
	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		void Init() override;
		void Shutdown() override;
		void SetClearColor(float r, float g, float b, float a) override;
		void Clear() override;
		void ClearStencil() override;
		void SetBlendFunction(BlendOption source, BlendOption destination) override;
		void SetStencilFunc(StencilOption func, int reference, unsigned int mask) override;
		void SetStencilMask(unsigned int mask) override;
		void SetStencilOp(StencilOption stencilFail, StencilOption zFail, StencilOption zPass) override;
		void EnableBlend(bool enable) override;
		void EnableDepthTest(bool enable) override;
		void EnableStencilTest(bool enable) override;

		void DrawIndexed(const std::shared_ptr<VertexArray> vertexArray, const std::shared_ptr<Shader> shader, unsigned int indexCount = 0) override;
		void DrawIndexedInstanced(const std::shared_ptr<VertexArray> vertexArray, const std::shared_ptr<Shader> shader, unsigned int instanceCount, unsigned int indexCount = 0) override;
	};
}