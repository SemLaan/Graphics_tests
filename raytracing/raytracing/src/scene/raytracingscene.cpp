#include "raytracingscene.h"

namespace Engine
{

	void RaytracingScene::Init(unsigned int screenWidth, unsigned int screenHeight)
	{
		m_width = screenWidth;
		m_height = screenHeight;

		m_scene = SimpleScene(m_cam, m_width, m_height);
		m_imageData = new float[m_width * m_height * TEXTURE_CHANNELS];
		m_dividedImage = new unsigned char[m_width * m_height * TEXTURE_CHANNELS];

		unsigned int vbSize = 4 * 4;
		float* quadVertices = new float[vbSize]{
			-1, -1, 0, 1,
			1, -1, 1, 1,
			-1, 1, 0, 0,
			1, 1, 1, 0
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
		m_shader->SetInt("u_Texture", 1);

		m_texture = Renderer::Texture::Create(m_width, m_height, Renderer::TextureFormat::RGBA8);
		m_texture->Bind(1);
	}

	void RaytracingScene::Update()
	{
		if (m_sampleCounter < 10000)
		{
			Raytracing::RenderToArray(m_imageData, m_scene, m_cam, m_width, m_height, m_samples, m_sampleCounter);

			for (unsigned int i = 0; i < m_width * m_height * TEXTURE_CHANNELS; i++)
			{
				m_dividedImage[i] = (unsigned char) (sqrt(m_imageData[i] / (float)(1U + m_sampleCounter)) * 255.99f);
			}

			m_sampleCounter++;
		}
	}

	void RaytracingScene::Render()
	{
		m_texture->SubTextureData(m_dividedImage);
		m_texture->Bind(1);

		Renderer::RenderCommand::DrawIndexed(m_va, m_shader);
	}

	void RaytracingScene::Shutdown()
	{
		// write image to png file
		for (unsigned int i = 0; i < m_width * m_height * TEXTURE_CHANNELS; i++)
		{
			m_dividedImage[i] = (unsigned char)(sqrt(m_imageData[i] / m_sampleCounter) * 255);
		}

		stbi_write_png("image.png", m_width, m_height, TEXTURE_CHANNELS, m_dividedImage, m_width * TEXTURE_CHANNELS);

		delete m_scene;
		delete[] m_imageData;
		delete[] m_dividedImage;
	}
}