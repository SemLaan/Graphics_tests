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
	}

	void RaytracingScene::Update()
	{
		Raytracing::RenderToArray(m_imageData, m_scene, m_cam, m_width, m_height, m_samples);

		for (unsigned int i = 0; i < m_width * m_height * TEXTURE_CHANNELS; i++)
		{
			m_dividedImage[i] = m_imageData[i] / (1 + m_sampleCounter);
		}

		m_sampleCounter++;
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