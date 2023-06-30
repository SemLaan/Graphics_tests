#pragma once
#include <memory>
#include <vector>
#include <string>
#include <glm/glm.hpp>

namespace Renderer
{
	// ------------------------------------------- ENUMS & CONVERSIONS -------------------------------------------------------
#pragma region ENUMS & CONVERSIONS
	// All possible shader data types
	enum class ShaderDataType
	{
		None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};

	// Byte sizes of all shader data types
	static unsigned int ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:    return 4;
		case ShaderDataType::Float2:   return 4 * 2;
		case ShaderDataType::Float3:   return 4 * 3;
		case ShaderDataType::Float4:   return 4 * 4;
		case ShaderDataType::Mat3:     return 4 * 3 * 3;
		case ShaderDataType::Mat4:     return 4 * 4 * 4;
		case ShaderDataType::Int:      return 4;
		case ShaderDataType::Int2:     return 4 * 2;
		case ShaderDataType::Int3:     return 4 * 3;
		case ShaderDataType::Int4:     return 4 * 4;
		case ShaderDataType::Bool:     return 1;
		}

		return 0;
	}

	// Enum of all possible frame buffer texture formats
	enum class FramebufferTextureFormat
	{
		None = 0,

		// Color
		RGBA8,
		R8,

		// Depth/stencil
		DEPTH24STENCIL8,
		DEPTH = DEPTH24STENCIL8
	};

	// Enum of all possible texture formats
	enum class TextureFormat
	{
		None = 0, RGBA8, R8,
	};
#pragma endregion

	// ------------------------------------------- VERTEX BUFFER ELEMENT AND LAYOUT ------------------------------------------
#pragma region VERTEX BUFFER ELEMENT AND LAYOUT
	// Struct with all the information necessary to describe a single vertex attribute
	struct VertexBufferElement
	{
		ShaderDataType type;    // Datatype
		unsigned int size;      // Size of the datatype in bytes
		unsigned int offset;    // How many bytes from the start of the vertex to the start of this element
		unsigned int instanced; // Should this vertex atrib be instanced
		bool normalized;        // Should this element be normalized when it is used

		VertexBufferElement(ShaderDataType dataType, unsigned int offset, unsigned int instanced = 0, bool normalized = false)
			: type(dataType), size(ShaderDataTypeSize(dataType)), offset(offset), instanced(instanced), normalized(normalized)
		{
		}

		unsigned int GetComponentCount() const
		{
			switch (type)
			{
			case ShaderDataType::Float:   return 1;
			case ShaderDataType::Float2:  return 2;
			case ShaderDataType::Float3:  return 3;
			case ShaderDataType::Float4:  return 4;
			case ShaderDataType::Mat3:    return 3; // 3* float3
			case ShaderDataType::Mat4:    return 4; // 4* float4
			case ShaderDataType::Int:     return 1;
			case ShaderDataType::Int2:    return 2;
			case ShaderDataType::Int3:    return 3;
			case ShaderDataType::Int4:    return 4;
			case ShaderDataType::Bool:    return 1;
			}
			return 0;
		}
	};

	// Class that holds all the vertex attributes that make up a vertex
	class VertexBufferLayout
	{
	private:
		/// <summary>
		/// All the elements that make up 1 vertex, for example (float 2 position, float4 color)
		/// </summary>
		std::vector<VertexBufferElement> m_elements;
		/// <summary>
		/// Total size of one vertex in bytes
		/// </summary>
		unsigned int m_stride;
	public:
		VertexBufferLayout() : m_stride(0) {};

		void Push(ShaderDataType dataType, unsigned int instanced = 0, bool normalized = false)
		{
			m_elements.push_back({ dataType, m_stride, instanced, normalized });
			m_stride += ShaderDataTypeSize(dataType);
		}

		inline const std::vector<VertexBufferElement> GetElements() const { return m_elements; }
		inline unsigned int GetStride() const { return m_stride; }
	};
#pragma endregion

	// ------------------------------------------- VERTEX BUFFER -------------------------------------------------------------
#pragma region VERTEX BUFFER
	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;

		virtual void Set(const void* data, unsigned int size) = 0;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const VertexBufferLayout& GetLayout() const = 0;
		virtual void SetLayout(const VertexBufferLayout& layout) = 0;

		/// <summary>
		/// Creates an empty vertex buffer and reserves memory for it on the GPU
		/// </summary>
		/// <param name="size">Size of the buffer in bytes</param>
		/// <returns></returns>
		static std::shared_ptr<VertexBuffer> Create(unsigned int size);
		/// <summary>
		/// Creates a vertex buffer and fills it with the given data
		/// </summary>
		/// <param name="data"></param>
		/// <param name="size">Size of the buffer in bytes</param>
		/// <returns></returns>
		static std::shared_ptr<VertexBuffer> Create(const void* data, unsigned int size);
	};
#pragma endregion
	
	// ------------------------------------------- INDEX BUFFER --------------------------------------------------------------
#pragma region INDEX BUFFER
	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() = default;

		virtual void Set(const unsigned int* data, unsigned int count) = 0;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual inline unsigned int GetCount() const = 0;

		/// <summary>
		/// Creates an empty index buffer and reserves memory for it on the GPU
		/// </summary>
		/// <param name="count">Amount of indices in the buffer (NOT in bytes)</param>
		/// <returns></returns>
		static std::shared_ptr<IndexBuffer> Create(unsigned int count);
		/// <summary>
		/// Creates a vertex buffer and fills it with the given data
		/// </summary>
		/// <param name="data"></param>
		/// <param name="count">Amount of indices in the buffer (NOT in bytes)</param>
		/// <returns></returns>
		static std::shared_ptr<IndexBuffer> Create(const unsigned int* data, unsigned int count);
	};
#pragma endregion
	
	// ------------------------------------------- VERTEX ARRAY --------------------------------------------------------------
#pragma region VERTEX ARRAY
	class VertexArray
	{
	public:
		virtual ~VertexArray() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer> vertexBuffer) = 0;
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer> indexBuffer) = 0;

		virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const = 0;
		virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const = 0;

		/// <summary>
		/// The vertex array holds the vertex buffer(s) and index buffer
		/// </summary>
		/// <returns></returns>
		static std::shared_ptr<VertexArray> Create();
	};
#pragma endregion
	
	// ------------------------------------------- SHADER --------------------------------------------------------------------
#pragma region SHADER
	struct ShaderProgramSource
	{
		std::string VertexSource;
		std::string FragmentSource;
	};

	class Shader
	{
	private:
		/// <summary>
		/// Compiles the shader
		/// </summary>
		/// <param name="type"></param>
		/// <param name="source"></param>
		/// <returns></returns>
		virtual unsigned int CompileShader(unsigned int type, const char* source) = 0;
		/// <summary>
		/// Puts the vertex and fragment shader together
		/// </summary>
		/// <param name="vertexShader"></param>
		/// <param name="fragmentShader"></param>
		/// <returns></returns>
		virtual unsigned int CreateShader(const char* vertexShader, const char* fragmentShader) = 0;
	protected:
		/// <summary>
		/// Reads in the shader code from the given file
		/// </summary>
		/// <param name="filepath"></param>
		/// <returns></returns>
		static ShaderProgramSource ParseShader(const std::string& filepath, bool alphaOutput = false);
	public:
		virtual ~Shader() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void SetInt(const std::string& name, int value) = 0;
		virtual void SetFloat(const std::string& name, float value) = 0;
		virtual void SetFloat2(const std::string& name, const glm::vec2& value) = 0;
		virtual void SetFloat3(const std::string& name, const glm::vec3& value) = 0;
		virtual void SetFloat4(const std::string& name, const glm::vec4& value) = 0;
		virtual void SetMat2(const std::string& name, const glm::mat2& mat) = 0;
		virtual void SetMat3(const std::string& name, const glm::mat3& mat) = 0;
		virtual void SetMat4(const std::string& name, const glm::mat4& mat) = 0;

		virtual const std::string& GetName() const = 0;

		/// <summary>
		/// Creates a shader based on the shader code in the given file
		/// </summary>
		/// <param name="filepath"></param>
		/// <returns></returns>
		static std::shared_ptr<Shader> Create(const std::string& filepath, bool alphaOutput = false);
	};
#pragma endregion
	
	// ------------------------------------------- TEXTURE -------------------------------------------------------------------
#pragma region TEXTURE
	class Texture
	{
	public:
		virtual ~Texture() = default;

		/// <summary>
		/// Binds the texture to the given slot (default is 0)
		/// </summary>
		/// <param name="slot"></param>
		virtual void Bind(unsigned int slot = 0) const = 0;
		virtual void UnbindSlotZero() const = 0;
		virtual void SubTextureData(unsigned char* data) = 0;

		virtual int GetWidth(unsigned int slot = 0) const = 0;
		virtual int GetHeight() const = 0;

		/// <summary>
		/// Creates an empty texture with the given width, height and texture format
		/// </summary>
		/// <param name="width"></param>
		/// <param name="height"></param>
		/// <param name="textureFormat"></param>
		/// <returns></returns>
		static std::shared_ptr<Texture> Create(int width, int height, TextureFormat textureFormat);
		/// <summary>
		/// Creates a texture with the given width, height, texture format and given data
		/// </summary>
		/// <param name="width"></param>
		/// <param name="height"></param>
		/// <param name="textureFormat"></param>
		/// <param name="data"></param>
		/// <returns></returns>
		static std::shared_ptr<Texture> Create(int width, int height, TextureFormat textureFormat, unsigned char* data);
		/// <summary>
		/// Creates a texture based on the given image file
		/// </summary>
		/// <param name="path"></param>
		/// <returns></returns>
		static std::shared_ptr<Texture> Create(const std::string& path);
		static std::shared_ptr<Texture> Create(unsigned int rendererID);
	};
#pragma endregion

	// ------------------------------------------- FRAMEBUFFER SPECIFICATION & FRAMEBUFFER -----------------------------------
#pragma region FRAMEBUFFER SPECIFICATION & FRAMEBUFFER
	/// <summary>
	/// Specifies width, height, texture attachments and whether the framebuffer is a swap chain target
	/// </summary>
	struct FramebufferSpecification
	{
		unsigned int width, height;
		std::vector<FramebufferTextureFormat> attachments;

		bool swapChainTarget = false;
	};


	class Framebuffer
	{
	public:
		virtual ~Framebuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		/// <summary>
		/// Binds one of the color attachments (texture) of the frame buffer to a given texture slot
		/// </summary>
		/// <param name="colorAttachmentIndex">Which color attachment to bind</param>
		/// <param name="slot">What texture slot to bind it to</param>
		/// <returns></returns>
		virtual unsigned int BindColorAttachment(unsigned int colorAttachmentIndex = 0, unsigned int slot = 0) const = 0;

		virtual std::shared_ptr<Texture> GetColorAttachmentAsTexture(unsigned int index = 0) const = 0;

		static std::shared_ptr<Framebuffer> Create(const FramebufferSpecification& specification);
	};
#pragma endregion
}
