#pragma once
#include "scene.h"
#include "raytracing/raytracinginclude.h"
#include "utils/utils.h"
#include "utils/scenes.h"
#include "rendering/rendererapi/rendererobjects.h"
#include "rendering/rendererapi/rendererapi.h"
#include "rendering/rendererapi/rendercommand.h"

#include <stb/stb_image_write.h>


namespace Engine
{

	class RaytracingScene : public Scene
	{
	public:
		RaytracingScene() = default;
		~RaytracingScene() = default;

		// Inherited via Scene
		void Init(unsigned int screenWidth, unsigned int screenHeight) override;
		void Update() override;
		void Render() override;
		void Shutdown() override;

	private:

		// Rendering
		std::shared_ptr<Renderer::Shader> m_shader;
		std::shared_ptr<Renderer::Texture> m_texture;
		std::shared_ptr<Renderer::VertexArray> m_va;

		// Raytracing
		unsigned int m_width;
		unsigned int m_height;
		int m_samples = 1; // samples per pixel
		Camera m_cam;
		Hitable* m_scene;
		unsigned int* m_imageData;
		unsigned char* m_dividedImage;
		int m_sampleCounter = 0;
	};
}