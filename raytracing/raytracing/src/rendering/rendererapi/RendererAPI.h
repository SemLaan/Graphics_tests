#pragma once
#include <memory>
#include "rendering/rendererapi/RendererObjects.h"

namespace Renderer
{
	enum class BlendOption
	{
		ZERO, 
		ONE, 
		SRC_COLOR, 
		ONE_MINUS_SRC_COLOR, 
		DST_COLOR, 
		ONE_MINUS_DST_COLOR, 
		SRC_ALPHA, 
		ONE_MINUS_SRC_ALPHA, 
		DST_ALPHA, 
		ONE_MINUS_DST_ALPHA,
		CONSTANT_COLOR, 
		ONE_MINUS_CONSTANT_COLOR, 
		CONSTANT_ALPHA, 
		ONE_MINUS_CONSTANT_ALPHA
	};
	enum class StencilOption 
	{
		NEVER,
		INCR,
		KEEP,
		DECR,
		EQUAL,
		ALWAYS,
	};

	class RendererAPI
	{
	public:
		enum class API
		{
			None = 0, OpenGL = 1
		};

		
	private:
		static API s_API;
	protected:
		int drawCallCount = 0;
	public:
		virtual ~RendererAPI() = default;
		
		int GetDrawCallsAndReset()
		{
			int temp = drawCallCount;
			drawCallCount = 0;
			return temp;
		}

		virtual void Init() = 0;
		virtual void Shutdown() = 0;
		virtual void SetClearColor(float r, float g, float b, float a) = 0;
		virtual void Clear() = 0;
		virtual void ClearStencil() = 0;
		virtual void SetBlendFunction(BlendOption source, BlendOption destination) = 0;
		virtual void SetStencilFunc(StencilOption func, int reference, unsigned int mask) = 0;
		virtual void SetStencilMask(unsigned int mask) = 0;
		virtual void SetStencilOp(StencilOption fail, StencilOption zfail, StencilOption zpass) = 0;
		virtual void EnableBlend(bool enable) = 0;
		virtual void EnableDepthTest(bool enable) = 0;
		virtual void EnableStencilTest(bool enable) = 0;

		/// <summary>
		/// Draw call for one non-instanced object (automatically gets the vertex and index buffer from the vertex array)
		/// </summary>
		/// <param name="vertexArray"></param>
		/// <param name="shader"></param>
		/// <param name="indexCount"></param>
		virtual void DrawIndexed(const std::shared_ptr<VertexArray> vertexArray, const std::shared_ptr<Shader> shader, unsigned int indexCount = 0) = 0;

		/// <summary>
		/// Draw call for instanced objects (automatically gets the vertex and index buffer from the vertex array)
		/// </summary>
		/// <param name="vertexArray"></param>
		/// <param name="shader"></param>
		/// <param name="indexCount"></param>
		virtual void DrawIndexedInstanced(const std::shared_ptr<VertexArray> vertexArray, const std::shared_ptr<Shader> shader, unsigned int instanceCount, unsigned int indexCount = 0) = 0;

		/// <summary>
		/// Returns the API type that is currently being used not the api object itself
		/// </summary>
		/// <returns></returns>
		static API GetAPI() { return s_API; }
		static std::unique_ptr<RendererAPI> Create();
	};
}