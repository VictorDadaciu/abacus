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
		VkSurfaceCapabilitiesKHR capabilities{};
		std::vector<VkSurfaceFormatKHR> formats{};
		std::vector<VkPresentModeKHR> presentModes{};
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

	struct Device
	{
		VkPhysicalDevice physical{};
		VkDevice logical{};

		const std::vector<const char*> deviceExtensions = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};

		VkQueue graphicsQueue{};
		VkQueue presentQueue{};
	};

	struct Swapchain
	{
		VkSwapchainKHR sc{};
		std::vector<VkImage> images{};
		std::vector<VkImageView> imageViews{};
		VkFormat imageFormat{};
		VkExtent2D extent{};
	};

	class GameObject;
	class Shader;
	class GraphicsRenderer
	{
	public:
		static GraphicsRenderer* GetInstance();
		void Initialise();

		//getters
		const int GetCurrentFrameIndex() const { return m_currentFrame; }
		void WindowResized() { m_framebufferResized = true; }

		void DrawFrame();
		void WaitForDeviceToIdle();

		void LoadModel();
		VkFormat FindDepthFormat();
		void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
		void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
		void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
		void CreateColorResources();
		void CreateDepthResources();
		VkImageView CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);
		void CreateImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMem, uint32_t mipLevels, VkSampleCountFlagBits numSamples);
		void GenerateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);
		void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);
		void CreateCommandBuffers();
		VkCommandBuffer BeginSingleTimeCommands();
		void EndSingleTimeCommands(VkCommandBuffer commandBuffer);

		const Device& GetDevice() const { return m_device; }
		const Swapchain& GetSwapchain() const { return m_swapchain; }
		const VkImageView& GetDepthImageView() const { return m_depthImageView; }
		const VkImageView& GetColorImageView() const { return m_colorImageView; }
		const VkCommandPool& GetCommandPool() const { return m_commandPool; }
		const VkSampleCountFlagBits GetMSAASamples() const { return m_msaaSamples; }

		const bool AreValidationLayersEnabled() const { return m_enableValidationLayers; }

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

		const int MAX_FRAMES_IN_FLIGHT = 3;

	protected:
		GraphicsRenderer();
		static GraphicsRenderer* st_instance;

#ifdef NDEBUG
		const bool m_enableValidationLayers = false;
#else
		const bool m_enableValidationLayers = true;
#endif

		//variables
		VkInstance m_vk{};
		VkSurfaceKHR m_surface{};
		Device m_device{};
		Swapchain m_swapchain{};
		VkCommandPool m_commandPool{};
		VkImage m_depthImage{};
		VkDeviceMemory m_depthImageMem{};
		VkImageView m_depthImageView{};
		VkImage m_colorImage{};
		VkDeviceMemory m_colorImageMem{};
		VkImageView m_colorImageView{};
		GameObject* m_go{};
		GameObject* m_cam{};
		Shader* m_shader{};
		VkSampleCountFlagBits m_msaaSamples = VK_SAMPLE_COUNT_1_BIT;
		std::vector<VkCommandBuffer> m_commandBuffers{};

		std::vector<VkSemaphore> m_imageAvailableSemaphores{};
		std::vector<VkSemaphore> m_renderFinishedSemaphores{};
		std::vector<VkFence> m_inFlightFences{};

		uint32_t m_currentFrame = 0;
		bool m_framebufferResized = false;

		VkDebugUtilsMessengerEXT m_debugMessenger{};

		// inits
		void CreateVulkanInstance();
		bool CheckValidationLayerSupport();
		std::vector<const char*> GetRequiredExtensions();
		void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
		void SetupDebugMessenger();
		void CreateSurface();
		void PickPhysicalDevice();
		bool IsPhysicalDeviceSuitable(VkPhysicalDevice physicalDevice);
		bool CheckDeviceExtensionSupport(VkPhysicalDevice physicalDevice);
		void CreateLogicalDevice();
		void CreateSwapchain();
		VkSampleCountFlagBits  GetMaxUsableSampleCount();
		VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
		VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
		void CreateImageViews();
		void CreateCommandPool();
		void CreateSyncObjects();

		QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice physicalDevice);
		SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice physicalDevice);
		uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
		VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiliing, VkFormatFeatureFlags features);
		const bool HasStencilComponent(VkFormat format) const { return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT; }

		void RecreateSwapchain();
		void CleanupSwapchain();
	};

#define RENDERER (GraphicsRenderer::GetInstance())
}

