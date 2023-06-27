#pragma once
#include "RendererAPI.h"

namespace Renderer
{
	
	class RenderCommand
	{
	private:
		static std::unique_ptr<RendererAPI> s_RendererAPI; // The renderer api object
	public:
		inline static int GetDrawCallsAndReset()
		{
			return s_RendererAPI->GetDrawCallsAndReset();
		}

		inline static void Init()
		{
			s_RendererAPI->Init();
		}

		inline static void Shutdown()
		{
			s_RendererAPI->Shutdown();
		}

		inline static void SetClearColor(float r, float g, float b, float a)
		{
			s_RendererAPI->SetClearColor(r, g, b, a);
		}

		inline static void Clear()
		{
			s_RendererAPI->Clear();
		}

		inline static void ClearStencil()
		{
			s_RendererAPI->ClearStencil();
		}

		inline static void SetBlendFunction(BlendOption source, BlendOption destination)
		{
			s_RendererAPI->SetBlendFunction(source, destination);
		}

		inline static void SetStencilFunc(StencilOption func, int reference, unsigned int mask)
		{
			s_RendererAPI->SetStencilFunc(func, reference, mask);
		}
		
		inline static void SetStencilMask(unsigned int mask)
		{
			s_RendererAPI->SetStencilMask(mask);
		}
		
		inline static void SetStencilOp(StencilOption stencilFail, StencilOption depthFail, StencilOption depthPass)
		{
			s_RendererAPI->SetStencilOp(stencilFail, depthFail, depthPass);
		}
		
		inline static void EnableBlend(bool enable)
		{
			s_RendererAPI->EnableBlend(enable);
		}
		
		inline static void EnableDepthTest(bool enable)
		{
			s_RendererAPI->EnableDepthTest(enable);
		}

		inline static void EnableStencilTest(bool enable)
		{
			s_RendererAPI->EnableStencilTest(enable);
		}
		
		/// <summary>
		/// Draw call for one non-instanced object (automatically gets the vertex and index buffer from the vertex array)
		/// </summary>
		/// <param name="vertexArray"></param>
		/// <param name="indexCount"></param>
		inline static void DrawIndexed(const std::shared_ptr<VertexArray> vertexArray, const std::shared_ptr<Shader> shader, unsigned int indexCount = 0)
		{
			s_RendererAPI->DrawIndexed(vertexArray, shader, indexCount);
		}

		inline static void DrawIndexedInstanced(const std::shared_ptr<VertexArray> vertexArray, const std::shared_ptr<Shader> shader, unsigned int instanceCount, unsigned int indexCount = 0)
		{
			s_RendererAPI->DrawIndexedInstanced(vertexArray, shader, instanceCount, indexCount);
		}
	};
}