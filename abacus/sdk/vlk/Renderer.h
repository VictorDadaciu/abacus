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

	class Framebuffer;
	class RenderPass;
	class Pipeline;
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
		const RenderPass* GetRenderPass() const { return m_renderPass; }
		const int GetCurrentFrameIndex() const { return m_currentFrame; }
		void WindowResized() { m_framebufferResized = true; }

		void DrawFrame();
		void WaitForDeviceToIdle();

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

		const int MAX_FRAMES_IN_FLIGHT = 2;

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
		RenderPass* m_renderPass{};
		Pipeline* m_graphicsPipeline{};
		std::vector<Framebuffer*> m_framebuffers{};
		VkCommandPool m_commandPool{};
		std::vector<VkCommandBuffer> m_commandBuffers{};

		std::vector<VkSemaphore> m_imageAvailableSemaphores{};
		std::vector<VkSemaphore> m_renderFinishedSemaphores{};
		std::vector<VkFence> m_inFlightFences{};

		uint32_t m_currentFrame = 0;
		bool m_framebufferResized = false;

		VkDebugUtilsMessengerEXT m_debugMessenger{};

		// inits
		void CreateInstance();
		bool CheckValidationLayerSupport();
		std::vector<const char*> GetRequiredExtensions();
		void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
		void SetupDebugMessenger();
		void CreateSurface();
		void CreateCommandPool();
		void CreateCommandBuffers();
		void CreateSyncObjects();

		void RecreateSwapchain();
		void CleanupSwapchain();

		void RecordCommandBuffer(VkCommandBuffer, uint32_t imageIndex);
	};
}

