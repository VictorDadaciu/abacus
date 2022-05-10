#pragma once

#include "vulkan.h"


namespace abc
{
	class Application;
	class Renderer
	{
	public:
		Renderer(Application* app);
		
		const VkInstance& Instance() const { return m_vk; }

		void Destroy();
	private:
		Application* m_app = nullptr;
		VkInstance m_vk{};

		void CreateInstance();
	};
}

