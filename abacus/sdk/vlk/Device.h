#pragma once

#include "Renderer.h"

namespace abc
{
	class Device
	{
	public:
		Device(Renderer* renderer);
		~Device();

		const VkPhysicalDevice& GetPhysical() const { return m_physical; }
		const VkDevice& GetLogical() const { return m_logical; }

		void Destroy();

	protected:
		Renderer* m_renderer{};
		VkPhysicalDevice m_physical{};
		VkDevice m_logical{};

		const std::vector<const char*> m_deviceExtensions = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};

		VkQueue m_graphicsQueue{};
		VkQueue m_presentQueue{};

		void PickPhysicalDevice();
		bool IsPhysicalDeviceSuitable(VkPhysicalDevice physicalDevice);
		bool CheckDeviceExtensionSupport(VkPhysicalDevice physicalDevice);

		void CreateLogicalDevice();

		friend class Renderer;
	};
}

