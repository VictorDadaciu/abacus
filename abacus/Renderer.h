#pragma once

#include "vulkan.h"

namespace abc
{
	struct QueueFamilyIndices
	{
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;

		const bool IsComplete() const { return graphicsFamily.has_value() && presentFamily.has_value(); }
	};

	struct SwapChainSupportDetails
	{
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	VkResult CreateDebugUtilsMessengerEXT(
		VkInstance instance, 								  
		const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, 
		const VkAllocationCallbacks* pAllocator, 
		VkDebugUtilsMessengerEXT* pDebugMessenger
	);

	void DestroyDebugUtilsMessengerEXT(
		VkInstance instance,
		VkDebugUtilsMessengerEXT debugMessenger,
		const VkAllocationCallbacks* pAllocator
	);

	class Swapchain;
	class Device;
	class Application;
	class Renderer
	{
	public:
		Renderer(Application* app);
		
		//getters
		const VkInstance& Instance() const { return m_vk; }
		const VkSurfaceKHR& GetSurface() const { return m_surface; }
		const Device* GetDevice() const { return m_device; }
		const Swapchain* GetSwapchain() const { return m_swapchain; }

		const bool AreValidationLayersEnabled() const { return m_enableValidationLayers; }

		QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice physicalDevice);
		SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice physicalDevice);

		void Destroy();

		//debug
		static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
			VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
			void* pUserData
		);

		const std::vector<const char*> m_validationLayers = {
			"VK_LAYER_KHRONOS_validation",
		};

	protected:

#ifdef NDEBUG
		const bool m_enableValidationLayers = false;
#else
		const bool m_enableValidationLayers = true;
#endif

		//variables
		Application* m_app{};
		VkInstance m_vk{};
		VkSurfaceKHR m_surface{};
		Device* m_device{};
		Swapchain* m_swapchain{};

		VkDebugUtilsMessengerEXT m_debugMessenger{};

		// inits
		void CreateInstance();
		bool CheckValidationLayerSupport();
		std::vector<const char*> GetRequiredExtensions();
		void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
		void SetupDebugMessenger();
		void CreateSurface();
	};
}

