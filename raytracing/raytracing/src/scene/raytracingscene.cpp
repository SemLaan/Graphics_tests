#include "raytracingscene.h"

namespace Engine
{

	void RaytracingScene::Init(unsigned int screenWidth, unsigned int screenHeight)
	{
		m_width = screenWidth;
		m_height = screenHeight;

		m_scene = RandomScene(m_cam, m_width, m_height);
		m_imageData = new unsigned int[m_width * m_height * TEXTURE_CHANNELS];
		m_dividedImage = new unsigned char[m_width * m_height * TEXTURE_CHANNELS];

		unsigned int vbSize = 4 * 4;
		float* quadVertices = new float[vbSize]{
			-1, -1, 0, 0,
			1, -1, 1, 0,
			-1, 1, 0, 1,
			1, 1, 1, 1
		};

		Renderer::VertexBufferLayout vbLayout;
		vbLayout.Push(Renderer::ShaderDataType::Float2);
		vbLayout.Push(Renderer::ShaderDataType::Float2);
		std::shared_ptr<Renderer::VertexBuffer> vb = Renderer::VertexBuffer::Create(quadVertices, sizeof(float) * vbSize);
		vb->SetLayout(vbLayout);

		unsigned int indicesCount = 6;
		unsigned int* quadIndices = new unsigned int[indicesCount] {
			0, 1, 2,
			2, 1, 3
		};
		std::shared_ptr<Renderer::IndexBuffer> ib = Renderer::IndexBuffer::Create(quadIndices, indicesCount);

		m_va = Renderer::VertexArray::Create();
		m_va->AddVertexBuffer(vb);
		m_va->SetIndexBuffer(ib);

		m_shader = Renderer::Shader::Create("src/rendering/shaders/texture.shader");
	}

	void RaytracingScene::Update()
	{
		/*
		Raytracing::RenderToArray(m_imageData, m_scene, m_cam, m_width, m_height, m_samples);

		for (unsigned int i = 0; i < m_width * m_height * TEXTURE_CHANNELS; i++)
		{
			m_dividedImage[i] = m_imageData[i] / (1 + m_sampleCounter);
		}
		*/
		m_sampleCounter++;
	}

	void RaytracingScene::Render()
	{
		// Screen frame buffer
		Renderer::FramebufferSpecification screenFrameBufferSpecification =
		{ 0, 0 , {Renderer::FramebufferTextureFormat::None}, true };
		std::shared_ptr<Renderer::Framebuffer> screenFramebuffer = Renderer::Framebuffer::Create(screenFrameBufferSpecification);
		screenFramebuffer->Bind();

		Renderer::RenderCommand::DrawIndexed(m_va, m_shader);
	}

	void RaytracingScene::Shutdown()
	{
		// write image to png file
		for (unsigned int i = 0; i < m_width * m_height * TEXTURE_CHANNELS; i++)
		{
			m_dividedImage[i] = m_imageData[i] / (1 + m_sampleCounter);
		}

		stbi_write_png("image.png", m_width, m_height, TEXTURE_CHANNELS, m_dividedImage, m_width * TEXTURE_CHANNELS);

		delete m_scene;
		delete[] m_imageData;
		delete[] m_dividedImage;
	}
}