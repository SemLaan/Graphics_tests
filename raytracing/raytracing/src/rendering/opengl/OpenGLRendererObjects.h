#pragma once
#include "rendering/rendererapi/RendererObjects.h"

#include <unordered_map>


namespace Renderer
{
	
	// ------------------------------------------- VERTEX BUFFER -------------------------------------------------------------
#pragma region VERTEX BUFFER
	class OpenGLVertexBuffer : public VertexBuffer
	{
	private:
		inline static unsigned int s_boundID = 0;
		unsigned int m_rendererID; //just an id for the buffer
		VertexBufferLayout m_layout;
	public:
		OpenGLVertexBuffer(unsigned int size);
		OpenGLVertexBuffer(const void* data, unsigned int size);

		virtual ~OpenGLVertexBuffer();

		const VertexBufferLayout& GetLayout() const override { return m_layout; }
		void SetLayout(const VertexBufferLayout& layout) override { m_layout = layout; }

		void Set(const void* data, unsigned int size) override;
		void Bind() const override;
		void Unbind() const override;
	};
#pragma endregion

	// ------------------------------------------- INDEX BUFFER --------------------------------------------------------------
#pragma region INDEX BUFFER
	class OpenGLIndexBuffer : public IndexBuffer
	{
	private:
		inline static unsigned int s_boundID = 0;
		unsigned int m_rendererID; //just an id for the buffer
		unsigned int m_count;
	public:
		OpenGLIndexBuffer(unsigned int count);
		OpenGLIndexBuffer(const unsigned int* data, unsigned int count);

		virtual ~OpenGLIndexBuffer();

		void Set(const unsigned int* data, unsigned int count) override;
		void Bind() const override;
		void Unbind() const override;

		inline unsigned int GetCount() const override { return m_count; };
	};
#pragma endregion

	// ------------------------------------------- VERTEX ARRAY --------------------------------------------------------------
#pragma region VERTEX ARRAY
	class OpenGLVertexArray : public VertexArray
	{
	private:
		inline static unsigned int s_boundID = 0;
		unsigned int m_rendererID;
		unsigned int m_vertexBufferIndex;
		std::vector<std::shared_ptr<VertexBuffer>> m_vertexBuffers;
		std::shared_ptr<IndexBuffer> m_indexBuffer;
	public:
		OpenGLVertexArray();

		virtual ~OpenGLVertexArray();

		void Bind() const override;
		void Unbind() const override;

		void AddVertexBuffer(const std::shared_ptr<VertexBuffer> vertexBuffer) override;
		void SetIndexBuffer(const std::shared_ptr<IndexBuffer> indexBuffer) override;

		const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const { return m_vertexBuffers; }
		const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const { return m_indexBuffer; }
	};
#pragma endregion

	// ------------------------------------------- SHADER --------------------------------------------------------------------
#pragma region SHADER
	class OpenGLShader : public Shader
	{
	private:
		inline static unsigned int s_boundID = 0;
		unsigned int m_rendererID;
		std::string m_filePath;
		std::unordered_map<std::string, int> m_UniformLocationCache; // like a hashtable

		int GetUniformLocation(const std::string name);

		unsigned int CompileShader(unsigned int type, const char* source) override;
		unsigned int CreateShader(const char* vertexShader, const char* fragmentShader) override;

	public:
		OpenGLShader(const std::string& filepath, bool alphaOutput);
		virtual ~OpenGLShader();

		void Bind() override;
		void Unbind() override;

		void SetInt(const std::string& name, int value) override;
		void SetFloat(const std::string& name, float value) override;
		void SetFloat2(const std::string& name, const glm::vec2& value) override;
		void SetFloat3(const std::string& name, const glm::vec3& value) override;
		void SetFloat4(const std::string& name, const glm::vec4& value) override;
		void SetMat2(const std::string& name, const glm::mat2& mat) override;
		void SetMat3(const std::string& name, const glm::mat3& mat) override;
		void SetMat4(const std::string& name, const glm::mat4& mat) override;

		const std::string& GetName() const override { return m_filePath; }
	};
#pragma endregion

	// ------------------------------------------- TEXTURE -------------------------------------------------------------------
#pragma region TEXTURE
	class OpenGLTexture : public Texture
	{
	private:
		TextureFormat m_textureFormat;
		unsigned int m_rendererID;
		std::string m_filePath;
		unsigned char* m_localBuffer;
		int m_width, m_height, m_BPP;
	public:
		OpenGLTexture(int width, int height, TextureFormat textureFormat);
		OpenGLTexture(int width, int height, TextureFormat textureFormat, unsigned char* data);
		OpenGLTexture(const std::string& path);
		OpenGLTexture(unsigned int rendererID);
		virtual ~OpenGLTexture();

		void Bind(unsigned int slot = 0) const override;
		void UnbindSlotZero() const override;
		void SubTextureData(unsigned char* data) override;

		void LoadImage();
		void GenerateTexture();

		int GetWidth(unsigned int slot = 0) const override { return m_width; }
		int GetHeight() const override { return m_height; }
	};
#pragma endregion

	// ------------------------------------------- FRAMEBUFFER SPECIFICATION & FRAMEBUFFER -----------------------------------
#pragma region FRAMEBUFFER SPECIFICATION & FRAMEBUFFER
	class OpenGLFramebuffer : public Framebuffer
	{
	private:
		inline static unsigned int s_boundID = 0;
		unsigned int m_rendererID;
		std::vector<unsigned int> m_colorAttachments;
		FramebufferSpecification m_specification;
		unsigned int m_attachmentCount;
	public:
		OpenGLFramebuffer(FramebufferSpecification specification);
		virtual ~OpenGLFramebuffer();

		void Bind() const override;
		void Unbind() const override;

		unsigned int BindColorAttachment(unsigned int colorAttachmentIndex = 0, unsigned int slot = 0) const override;
		std::shared_ptr<Texture> GetColorAttachmentAsTexture(unsigned int index = 0) const override;
	};
#pragma endregion
	
}
