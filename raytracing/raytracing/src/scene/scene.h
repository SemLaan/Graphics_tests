#pragma once

namespace Engine
{

	class Scene
	{
	public:
		Scene() = default;
		~Scene() = default;

		virtual void Init(unsigned int screenWidth, unsigned int screenHeight) = 0;
		virtual void Update() = 0;
		virtual void Render() = 0;
		virtual void Shutdown() = 0;
	};
}