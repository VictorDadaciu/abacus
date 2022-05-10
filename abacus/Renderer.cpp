#include "pch.h"
#include "Renderer.h"

#include "Application.h"

namespace abc
{
	Renderer::Renderer(Application* app) :
		m_app(app)
	{
		CreateInstance();
	}

	void Renderer::CreateInstance()
	{
		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Triangle";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "Abacus";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_3;


		uint32_t sdlExtensionCount = 0;
		SDL_Vulkan_GetInstanceExtensions(m_app->GetActiveWindow()->Raw(), &sdlExtensionCount, nullptr);
		std::vector<const char*> sdlExtensions(sdlExtensionCount);
		SDL_Vulkan_GetInstanceExtensions(m_app->GetActiveWindow()->Raw(), &sdlExtensionCount, sdlExtensions.data());


		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;
		createInfo.enabledExtensionCount = sdlExtensions.size();
		createInfo.ppEnabledExtensionNames = sdlExtensions.data();
		createInfo.enabledLayerCount = 0;

		if (vkCreateInstance(&createInfo, nullptr, &m_vk) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create instance!");
		}
	}

	void Renderer::Destroy()
	{
		vkDestroyInstance(m_vk, nullptr);
	}
}