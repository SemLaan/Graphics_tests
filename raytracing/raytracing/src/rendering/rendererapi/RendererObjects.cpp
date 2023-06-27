#include "RendererObjects.h"

//Including opengl objects
#include "rendering/opengl/OpenGLRendererObjects.h"

// Other includes
#include <fstream>
#include <sstream>


namespace Renderer
{
	// ---------------------------------------- PARSE SHADERS -------------------------------------------
	ShaderProgramSource Shader::ParseShader(const std::string& filepath, bool alphaOutput)
	{
		enum class ShaderType
		{
			NONE = -1, VERTEX = 0, FRAGMENT = 1
		};

		std::ifstream stream(filepath);
		std::string line;
		std::stringstream ss[2];
		ShaderType type = ShaderType::NONE;

		while (getline(stream, line))
		{
			if (line.find("#shader") != std::string::npos)
			{
				if (line.find("vertex") != std::string::npos)
					type = ShaderType::VERTEX;
				else if (line.find("fragment") != std::string::npos)
					type = ShaderType::FRAGMENT;
			}
			else //if (type != ShaderType::NONE)
			{
				if (alphaOutput)
				{
					if (line.find("out vec4 color") != std::string::npos)
						line.replace(line.find("out vec4 color"), 14, "out float color");
					else if (line.find("color = ") != std::string::npos)
						line.replace(line.find(";"), 0, ".w");
				}
				ss[(int)type] << line << "\n";
			}
		}
		return { ss[0].str(), ss[1].str() };
	}
	

	// ---------------------------------------- CREATE FUNCTIONS ----------------------------------------
#pragma region CREATE FUNCTIONS
	std::shared_ptr<VertexBuffer> VertexBuffer::Create(unsigned int size)
	{
		std::shared_ptr<VertexBuffer> vertexBuffer(new OpenGLVertexBuffer(size));
		return vertexBuffer;
	}

	std::shared_ptr<VertexBuffer> VertexBuffer::Create(const void* data, unsigned int size)
	{
		std::shared_ptr<VertexBuffer> vertexBuffer(new OpenGLVertexBuffer(data, size));
		return vertexBuffer;
	}

	std::shared_ptr<IndexBuffer> IndexBuffer::Create(unsigned int count)
	{
		std::shared_ptr<IndexBuffer> indexBuffer(new OpenGLIndexBuffer(count));
		return indexBuffer;
	}

	std::shared_ptr<IndexBuffer> IndexBuffer::Create(const unsigned int* data, unsigned int count)
	{
		std::shared_ptr<IndexBuffer> indexBuffer(new OpenGLIndexBuffer(data, count));
		return indexBuffer;
	}
	
	std::shared_ptr<VertexArray> VertexArray::Create()
	{
		std::shared_ptr<VertexArray> vertexArray(new OpenGLVertexArray());
		return vertexArray;
	}

	std::shared_ptr<Framebuffer> Framebuffer::Create(const FramebufferSpecification& specification)
	{
		return std::make_shared<OpenGLFramebuffer>(specification);
	}

	std::shared_ptr<Renderer::Shader> Shader::Create(const std::string& filepath, bool alphaOutput)
	{
		return std::make_shared<Renderer::OpenGLShader>(filepath, alphaOutput);
	}

	std::shared_ptr<Texture> Texture::Create(int width, int height, TextureFormat textureFormat)
	{
		return std::make_shared<OpenGLTexture>(width, height, textureFormat);
	}

	std::shared_ptr<Texture> Texture::Create(int width, int height, TextureFormat textureFormat, unsigned char* data)
	{
		return std::make_shared<OpenGLTexture>(width, height, textureFormat, data);
	}

	std::shared_ptr<Texture> Texture::Create(const std::string& path)
	{
		return std::make_shared<OpenGLTexture>(path);
	}

	std::shared_ptr<Texture> Texture::Create(unsigned int rendererID)
	{
		return std::make_shared<OpenGLTexture>(rendererID);
	}
#pragma endregion
}