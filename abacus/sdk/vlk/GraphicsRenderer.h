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

	struct Pipeline
	{
		std::vector<char> vertCode{};
		std::vector<char> fragCode{};
		VkShaderModule vertModule{};
		VkShaderModule fragModule{};
		VkPipelineLayout layout{};
		VkPipeline pl{};
	};

	struct Vertex
	{
		glm::vec3 pos;
		glm::vec3 color;
		glm::vec2 uv;

		static VkVertexInputBindingDescription GetBindingDescription();
		static std::array<VkVertexInputAttributeDescription, 3> GetAttributeDescriptions();
	};

	struct UniformBufferObject
	{
		alignas(16) glm::mat4 model;
		alignas(16) glm::mat4 view;
		alignas(16) glm::mat4 proj;
	};

	class Buffer;
	class GraphicsRenderer
	{
	public:
		static GraphicsRenderer* GetInstance();

		//getters
		const int GetCurrentFrameIndex() const { return m_currentFrame; }
		void WindowResized() { m_framebufferResized = true; }

		void UpdateUniformBuffer(uint32_t currentImage);
		void DrawFrame();
		void WaitForDeviceToIdle();

		void CreateVertexBuffer();
		void CreateIndexBuffer();
		void CreateUniformBuffers();
		void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
		void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
		void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
		void CreateTextureImage();
		void CreateTextureImageView();
		void CreateTextureSampler();
		void CreateDepthResources();
		VkImageView CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
		void CreateImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMem);
		void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
		void CreateCommandBuffers();
		VkCommandBuffer BeginSingleTimeCommands();
		void EndSingleTimeCommands(VkCommandBuffer commandBuffer);

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
		VkRenderPass m_renderPass{};
		VkDescriptorSetLayout m_descriptorSetLayout;
		Pipeline m_graphicsPipeline{};
		std::vector<VkFramebuffer> m_framebuffers{};
		VkCommandPool m_commandPool{};
		VkImage m_image{};
		VkDeviceMemory m_imageMem{};
		VkImageView m_imageView{};
		VkImage m_depthImage{};
		VkDeviceMemory m_depthImageMem{};
		VkImageView m_depthImageView{};
		VkSampler m_sampler{};
		Buffer* m_vertexBuffer{};
		Buffer* m_indexBuffer{};
		std::vector<VkBuffer> m_uniformBuffers{};
		std::vector<VkDeviceMemory> m_uniformBuffersMemory{};
		VkDescriptorPool m_descriptorPool{};
		std::vector<VkDescriptorSet> m_descriptorSets{};
		std::vector<VkCommandBuffer> m_commandBuffers{};

		std::vector<VkSemaphore> m_imageAvailableSemaphores{};
		std::vector<VkSemaphore> m_renderFinishedSemaphores{};
		std::vector<VkFence> m_inFlightFences{};

		uint32_t m_currentFrame = 0;
		bool m_framebufferResized = false;

		VkDebugUtilsMessengerEXT m_debugMessenger{};

		const std::vector<Vertex> vertices = {
			{{-0.5f, -0.5f, 0.0f}, {0.98f, 0.1f, 0.1f}, {1.0f, 0.0f}},
			{{0.5f, -0.5f, 0.0f}, {0.1f, 0.98f, 0.1f}, {0.0f, 0.0f}},
			{{0.5f, 0.5f, 0.0f}, {0.1f, 0.1f, 0.98f}, {0.0f, 1.0f}},
			{{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},

			{{-0.5f, -0.5f, -0.5f}, {0.98f, 0.1f, 0.1f}, {1.0f, 0.0f}},
			{{0.5f, -0.5f, -0.5f}, {0.1f, 0.98f, 0.1f}, {0.0f, 0.0f}},
			{{0.5f, 0.5f, -0.5f}, {0.1f, 0.1f, 0.98f}, {0.0f, 1.0f}},
			{{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}
		};

		const std::vector<uint16_t> indices = {
			0, 1, 2, 2, 3, 0,
			4, 5, 6, 6, 7, 4
		};

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
		VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
		VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
		void CreateImageViews();
		void CreateRenderPass();
		void CreateDescriptorSetLayout();
		void CreateDescriptorPool();
		void CreateDescriptorSets();
		void CreatePipeline();
		VkShaderModule CreateShaderModule(const std::vector<char>& code);
		void CreateFramebuffers();
		void CreateCommandPool();
		void CreateSyncObjects();

		QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice physicalDevice);
		SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice physicalDevice);
		uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
		VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiliing, VkFormatFeatureFlags features);
		VkFormat FindDepthFormat();
		const bool HasStencilComponent(VkFormat format) const { return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT; }

		void RecreateSwapchain();
		void CleanupSwapchain();

		void RecordCommandBuffer(VkCommandBuffer, uint32_t imageIndex);
	};

#define RENDERER (GraphicsRenderer::GetInstance())
}

