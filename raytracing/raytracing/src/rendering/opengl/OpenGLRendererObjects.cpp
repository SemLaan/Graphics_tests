#include "openglrendererobjects.h"
#include "openglerrormanagement.h"

#include <glew/glew.h>
#include <stb/stb_image.h>
#include <iostream>

namespace Renderer
{

	// ------------------------------------------- ENUMS & CONVERSIONS -------------------------------------------------------
#pragma region ENUMS & CONVERSIONS
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:    return GL_FLOAT;
		case ShaderDataType::Float2:   return GL_FLOAT;
		case ShaderDataType::Float3:   return GL_FLOAT;
		case ShaderDataType::Float4:   return GL_FLOAT;
		case ShaderDataType::Mat3:     return GL_FLOAT;
		case ShaderDataType::Mat4:     return GL_FLOAT;
		case ShaderDataType::Int:      return GL_INT;
		case ShaderDataType::Int2:     return GL_INT;
		case ShaderDataType::Int3:     return GL_INT;
		case ShaderDataType::Int4:     return GL_INT;
		case ShaderDataType::Bool:     return GL_BOOL;
		}

		return 0;
	}
#pragma endregion

	// ------------------------------------------- VERTEX BUFFER -------------------------------------------------------------
#pragma region VERTEX BUFFER
	OpenGLVertexBuffer::OpenGLVertexBuffer(unsigned int size)
	{
		G(glGenBuffers(1, &m_rendererID));
		Bind();
		G(glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW));
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(const void* data, unsigned int size)
	{
		G(glGenBuffers(1, &m_rendererID));
		Bind();
		G(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		if (s_boundID == m_rendererID)
			s_boundID = 0;
		G(glDeleteBuffers(1, &m_rendererID));
	}

	void OpenGLVertexBuffer::Set(const void* data, unsigned int size)
	{
		Bind();
		G(glBufferSubData(GL_ARRAY_BUFFER, 0, size, data));
	}

	void OpenGLVertexBuffer::Bind() const
	{
		if (s_boundID != m_rendererID)
		{
			G(glBindBuffer(GL_ARRAY_BUFFER, m_rendererID));
			s_boundID = m_rendererID;
		}
	}

	void OpenGLVertexBuffer::Unbind() const
	{
		if (s_boundID != 0)
		{
			G(glBindBuffer(GL_ARRAY_BUFFER, 0));
			s_boundID = 0;
		}
	}
#pragma endregion

	// ------------------------------------------- INDEX BUFFER --------------------------------------------------------------
#pragma region INDEX BUFFER
	OpenGLIndexBuffer::OpenGLIndexBuffer(unsigned int count)
		: m_count(count)
	{
		G(glGenBuffers(1, &m_rendererID));
		Bind();
		G(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), nullptr, GL_DYNAMIC_DRAW));
	}

	OpenGLIndexBuffer::OpenGLIndexBuffer(const unsigned int* data, unsigned int count)
		: m_count(count)
	{
		G(glGenBuffers(1, &m_rendererID));
		Bind();
		G(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		if (s_boundID == m_rendererID)
			s_boundID = 0;
		G(glDeleteBuffers(1, &m_rendererID));
	}

	void OpenGLIndexBuffer::Set(const unsigned int* data, unsigned int count)
	{
		Bind();
		m_count = count;
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, count * sizeof(unsigned int), data);
	}

	void OpenGLIndexBuffer::Bind() const
	{
		//if (s_boundID != m_rendererID)
		//{
		G(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID));
		s_boundID = m_rendererID;
		//}
	}

	void OpenGLIndexBuffer::Unbind() const
	{
		if (s_boundID != 0)
		{
			G(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
			s_boundID = 0;
		}
	}
#pragma endregion

	// ------------------------------------------- VERTEX ARRAY --------------------------------------------------------------
#pragma region VERTEX ARRAY
	OpenGLVertexArray::OpenGLVertexArray()
		: m_vertexBufferIndex(0)
	{
		G(glGenVertexArrays(1, &m_rendererID));
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		if (s_boundID == m_rendererID)
			s_boundID = 0;
		G(glDeleteVertexArrays(1, &m_rendererID));
	}

	void OpenGLVertexArray::Bind() const
	{
		if (s_boundID != m_rendererID)
		{
			G(glBindVertexArray(m_rendererID));
			s_boundID = m_rendererID;
		}
	}

	void OpenGLVertexArray::Unbind() const
	{
		if (s_boundID != 0)
		{
			G(glBindVertexArray(0));
			s_boundID = 0;
		}
	}

	void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer> vertexBuffer)
	{
		Bind();
		vertexBuffer->Bind();
		m_vertexBuffers.push_back(vertexBuffer);

		const VertexBufferLayout& layout = vertexBuffer->GetLayout();
		for (const VertexBufferElement& element : layout.GetElements())
		{
			if (!(element.type == ShaderDataType::Mat3 || element.type == ShaderDataType::Mat4)) {
				G(glEnableVertexAttribArray(m_vertexBufferIndex));
				G(glVertexAttribPointer(m_vertexBufferIndex, element.GetComponentCount(),
					ShaderDataTypeToOpenGLBaseType(element.type), element.normalized,
					layout.GetStride(), (const void*)element.offset));
				G(glVertexAttribDivisor(m_vertexBufferIndex, element.instanced));
				m_vertexBufferIndex++;
			}
			else
			{
				unsigned int count = element.GetComponentCount();
				for (unsigned int i = 0; i < count; i++)
				{
					G(glEnableVertexAttribArray(m_vertexBufferIndex));
					G(glVertexAttribPointer(m_vertexBufferIndex, count,
						ShaderDataTypeToOpenGLBaseType(element.type), element.normalized,
						layout.GetStride(), (const void*)(element.offset + sizeof(float) * count * i)));
					G(glVertexAttribDivisor(m_vertexBufferIndex, element.instanced));
					m_vertexBufferIndex++;
				}
			}
		}
	}

	void OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer> indexBuffer)
	{
		Bind();
		indexBuffer->Bind();
		m_indexBuffer = indexBuffer;
	}
#pragma endregion

	// ------------------------------------------- SHADER --------------------------------------------------------------------
#pragma region SHADER
	OpenGLShader::OpenGLShader(const std::string& filepath, bool alphaOutput)
		: m_filePath(filepath)
	{
		ShaderProgramSource source = Shader::ParseShader(filepath, alphaOutput);
		m_rendererID = CreateShader(source.VertexSource.c_str(), source.FragmentSource.c_str());
	}

	OpenGLShader::~OpenGLShader()
	{
		if (s_boundID == m_rendererID)
			s_boundID = 0;
		G(glDeleteProgram(m_rendererID));
	}


	void OpenGLShader::Bind()
	{
		if (s_boundID != m_rendererID)
		{
			G(glUseProgram(m_rendererID));
			s_boundID = m_rendererID;
		}
	}

	void OpenGLShader::Unbind()
	{
		if (s_boundID != 0)
		{
			G(glUseProgram(0));
			s_boundID = 0;
		}
	}

	void OpenGLShader::SetInt(const std::string& name, int value)
	{
		Bind();
		G(glUniform1i(GetUniformLocation(name), value));
	}

	void OpenGLShader::SetFloat(const std::string& name, float value)
	{
		Bind();
		G(glUniform1f(GetUniformLocation(name), value));
	}

	void OpenGLShader::SetFloat2(const std::string& name, const glm::vec2& value)
	{
		Bind();
		G(glUniform2f(GetUniformLocation(name), value.x, value.y));
	}

	void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& value)
	{
		Bind();
		G(glUniform3f(GetUniformLocation(name), value.x, value.y, value.z));
	}

	void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& value)
	{
		Bind();
		G(glUniform4f(GetUniformLocation(name), value.x, value.y, value.z, value.w));
	}

	void OpenGLShader::SetMat2(const std::string& name, const glm::mat2& mat)
	{
		Bind();
		G(glUniformMatrix2fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]));
	}

	void OpenGLShader::SetMat3(const std::string& name, const glm::mat3& mat)
	{
		Bind();
		G(glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]));
	}

	void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& mat)
	{
		Bind();
		G(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]));
	}

	int OpenGLShader::GetUniformLocation(const std::string name)
	{
		if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
			return m_UniformLocationCache[name];

		G(int location = glGetUniformLocation(m_rendererID, name.c_str()));
		if (location == -1)
		{
			std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;
			CUSTOMWARNING("at line: ");
		}

		m_UniformLocationCache[name] = location; // cache it
		return location;
	}

	unsigned int OpenGLShader::CompileShader(unsigned int type, const char* source)
	{
		unsigned int id = glCreateShader(type);
		const char* src = source; // source.c_str();
		G(glShaderSource(id, 1, &src, nullptr));
		G(glCompileShader(id));

		int result;
		G(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
		if (result == GL_FALSE)
		{
			int length;
			G(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
			char* message = (char*)malloc(length * sizeof(char));
			G(glGetShaderInfoLog(id, length, &length, message));
			std::cout << "Failed to compile shader: " << std::endl << message << std::endl;
			free(message);
		}
		return id;
	}

	unsigned int OpenGLShader::CreateShader(const char* vertexShader, const char* fragmentShader)
	{
		unsigned int program = glCreateProgram();
		unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
		unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

		G(glAttachShader(program, vs));
		G(glAttachShader(program, fs));
		G(glLinkProgram(program));

		int success;
		char infoLog[512];
		glGetProgramiv(program, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(program, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}

		G(glValidateProgram(program));

		G(glDeleteShader(vs));
		G(glDeleteShader(fs));

		return program;
	}
#pragma endregion

	// ------------------------------------------- TEXTURE -------------------------------------------------------------------
#pragma region TEXTURE
	OpenGLTexture::OpenGLTexture(int width, int height, TextureFormat textureFormat)
		: m_width(width), m_height(height), m_textureFormat(textureFormat), m_localBuffer(nullptr), m_filePath("")
	{
		GenerateTexture();
	}

	OpenGLTexture::OpenGLTexture(const std::string& path)
		: m_textureFormat(TextureFormat::RGBA8), m_filePath(path)
	{
		LoadImage();
		GenerateTexture();
	}

	OpenGLTexture::OpenGLTexture(int width, int height, TextureFormat textureFormat, unsigned char* data)
		: m_width(width), m_height(height), m_textureFormat(textureFormat), m_localBuffer(data), m_filePath("")
	{
		GenerateTexture();
	}

	OpenGLTexture::OpenGLTexture(unsigned int rendererID)
		: m_rendererID(rendererID)
	{

	}

	OpenGLTexture::~OpenGLTexture()
	{
		G(glDeleteTextures(1, &m_rendererID));
	}

	void OpenGLTexture::Bind(unsigned int slot) const
	{
		G(glActiveTexture(GL_TEXTURE0 + slot));
		G(glBindTexture(GL_TEXTURE_2D, m_rendererID));
	}

	void OpenGLTexture::UnbindSlotZero() const
	{
		G(glActiveTexture(GL_TEXTURE0));
		G(glBindTexture(GL_TEXTURE_2D, 0));
	}

	void OpenGLTexture::SubTextureData(unsigned char* data)
	{
		m_localBuffer = data;
		Bind(0);
		switch (m_textureFormat)
		{
		case TextureFormat::RGBA8:
			G(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, GL_RGBA, GL_UNSIGNED_BYTE, m_localBuffer));
			break;
		case TextureFormat::R8:
			G(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, GL_RED, GL_UNSIGNED_BYTE, m_localBuffer));
			break;
		}
	}

	void OpenGLTexture::LoadImage()
	{
		//the bottom left in openGL is 0,0,
		//Typically image formats start with 0,0 on left top, therefore we flip the pixel array vertically
		stbi_set_flip_vertically_on_load(1);

		//the pixel array on your cpu
		m_localBuffer = stbi_load(m_filePath.c_str(), &m_width, &m_height, &m_BPP, 4); // this function fills in the W,H and bits per pixel from the file. 4 = r,g,b,a
	}

	void OpenGLTexture::GenerateTexture()
	{
		G(glGenTextures(1, &m_rendererID));
		Bind(0);
		G(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		G(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		G(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		G(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		G(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
		switch (m_textureFormat)
		{
		case TextureFormat::RGBA8:
			G(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_localBuffer));
			break;
		case TextureFormat::R8:
			G(glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, m_width, m_height, 0, GL_RED, GL_UNSIGNED_BYTE, m_localBuffer));
			break;
		}
		UnbindSlotZero();
	}
#pragma endregion

	// ------------------------------------------- FRAMEBUFFER UTILS & FRAMEBUFFER -------------------------------------------
#pragma region FRAMEBUFFER UTILS & FRAMEBUFFER
	namespace FramebufferUtils
	{
		unsigned int GenerateTexture(FramebufferTextureFormat textureFormat, unsigned int width, unsigned int height)
		{
			unsigned int texture;
			if (textureFormat == FramebufferTextureFormat::DEPTH24STENCIL8)
			{
				glGenRenderbuffers(1, &texture);
				glBindRenderbuffer(GL_RENDERBUFFER, texture);
				glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
				glBindRenderbuffer(GL_RENDERBUFFER, 0);
			}
			else
			{
				G(glGenTextures(1, &texture));
				G(glActiveTexture(GL_TEXTURE0));
				G(glBindTexture(GL_TEXTURE_2D, texture));

				G(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
				G(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
				G(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
				G(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

				switch (textureFormat)
				{
				case FramebufferTextureFormat::RGBA8:
					G(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr));
					break;
				case FramebufferTextureFormat::R8:
					G(glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr));
					break;
				case FramebufferTextureFormat::DEPTH24STENCIL8:
					G(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr));
					break;
				}
			}
			return texture;
		}
	}


	OpenGLFramebuffer::OpenGLFramebuffer(FramebufferSpecification specification)
		: m_specification(specification)
	{
		if (specification.swapChainTarget)
		{
			m_rendererID = 0;
			m_attachmentCount = 0;
		}
		else
		{
			m_attachmentCount = specification.attachments.size();

			G(glGenFramebuffers(1, &m_rendererID));
			Bind();

			for (int i = 0; i < m_attachmentCount; i++)
			{
				FramebufferTextureFormat attachmentFormat = specification.attachments[i];

				unsigned int texture = FramebufferUtils::GenerateTexture(attachmentFormat, specification.width, specification.height);
				m_colorAttachments.push_back(texture);

				if (attachmentFormat == FramebufferTextureFormat::DEPTH24STENCIL8)
				{
					G(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, texture));
				}
				else
				{
					G(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0));
				}
			}
		}
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		for (unsigned int i = 0; i < m_attachmentCount; i++)
		{
			G(glDeleteTextures(1, &m_colorAttachments[i]));
		}
		if (s_boundID == m_rendererID)
			s_boundID = 0;
		G(glDeleteFramebuffers(1, &m_rendererID));
	}

	void OpenGLFramebuffer::Bind() const
	{
		if (s_boundID != m_rendererID)
		{
			G(glBindFramebuffer(GL_FRAMEBUFFER, m_rendererID));
			s_boundID = m_rendererID;
		}
	}

	void OpenGLFramebuffer::Unbind() const
	{
		if (s_boundID != 0)
		{
			G(glBindFramebuffer(GL_FRAMEBUFFER, 0));
			s_boundID = 0;
		}
	}

	unsigned int OpenGLFramebuffer::BindColorAttachment(unsigned int colorAttachmentIndex, unsigned int slot) const
	{
		G(glActiveTexture(GL_TEXTURE0 + slot));
		G(glBindTexture(GL_TEXTURE_2D, m_colorAttachments[colorAttachmentIndex]));
		return 0;
	}

	std::shared_ptr<Renderer::Texture> OpenGLFramebuffer::GetColorAttachmentAsTexture(unsigned int index /*= 0*/) const
	{
		return OpenGLTexture::Create(m_colorAttachments[index]);
	}
#pragma endregion
}