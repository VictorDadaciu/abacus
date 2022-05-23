#include "pch.h"
#include "GraphicsRenderer.h"

#include "Application.h"
#include "GameObject.h"
#include "Buffer.h"
#include "Component.h"
#include "RenderComponent.h"
#include "TransformComponent.h"
#include "CameraComponent.h"
#include "Texture.h"
#include "Model.h"
#include "Shader.h"

#include "FileReader.h"

#include <chrono>

namespace abc
{
	GraphicsRenderer* GraphicsRenderer::st_instance = nullptr;

	GraphicsRenderer* GraphicsRenderer::GetInstance()
	{
		if (!st_instance)
		{
			st_instance = new GraphicsRenderer();
		}
		return st_instance;
	}

	void GraphicsRenderer::Initialise()
	{
		CreateVulkanInstance();
		SetupDebugMessenger();
		CreateSurface();
		PickPhysicalDevice();
		CreateLogicalDevice();
		CreateSwapchain();
		CreateImageViews();
		CreateCommandPool();
		LoadModel();
		CreateCommandBuffers();
		CreateSyncObjects();
	}

	GraphicsRenderer::GraphicsRenderer()
	{
	}

	void GraphicsRenderer::CreateVulkanInstance()
	{
		if (m_enableValidationLayers && !CheckValidationLayerSupport())
		{
			throw std::runtime_error("Validation layers requested are not available!");
		}

		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Abacus";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "Abacus";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_3;


		std::vector<const char*> extensions = GetRequiredExtensions();

		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;
		createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		createInfo.ppEnabledExtensionNames = extensions.data();

		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
		if (m_enableValidationLayers)
		{
			createInfo.enabledLayerCount = static_cast<uint32_t>(m_validationLayers.size());
			createInfo.ppEnabledLayerNames = m_validationLayers.data();

			PopulateDebugMessengerCreateInfo(debugCreateInfo);
			createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
		}
		else
		{
			createInfo.enabledLayerCount = 0;
		}

		if (vkCreateInstance(&createInfo, nullptr, &m_vk) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create instance!");
		}
	}

	void GraphicsRenderer::PickPhysicalDevice()
	{
		m_device.physical = VK_NULL_HANDLE;

		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(m_vk, &deviceCount, nullptr);
		if (deviceCount == 0)
		{
			throw std::runtime_error("failed to find GPUs with Vulkan support!");
		}

		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(m_vk, &deviceCount, devices.data());

		for (const auto& device : devices)
		{
			if (IsPhysicalDeviceSuitable(device))
			{
				m_device.physical = device;
				m_msaaSamples = GetMaxUsableSampleCount();
				break;
			}
		}

		if (m_device.physical == VK_NULL_HANDLE)
		{
			throw std::runtime_error("Failed to find a suitable GPU!");
		}
	}

	bool GraphicsRenderer::IsPhysicalDeviceSuitable(VkPhysicalDevice physicalDevice)
	{
		QueueFamilyIndices indices = FindQueueFamilies(physicalDevice);

		bool extensionsSupported = CheckDeviceExtensionSupport(physicalDevice);

		bool swapChainAdequate = false;
		if (extensionsSupported)
		{
			SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(physicalDevice);
			swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
		}

		VkPhysicalDeviceFeatures supportedFeatures;
		vkGetPhysicalDeviceFeatures(physicalDevice, &supportedFeatures);

		return indices.IsComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
	}

	bool GraphicsRenderer::CheckDeviceExtensionSupport(VkPhysicalDevice physicalDevice)
	{
		uint32_t extensionCount;
		vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);

		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, availableExtensions.data());

		std::set<std::string> requiredExtensions(m_device.deviceExtensions.begin(), m_device.deviceExtensions.end());

		for (const auto& extension : availableExtensions)
		{
			requiredExtensions.erase(extension.extensionName);
		}

		return requiredExtensions.empty();
	}

	void GraphicsRenderer::CreateLogicalDevice()
	{
		QueueFamilyIndices indices = FindQueueFamilies(m_device.physical);

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

		float queuePriority = 1.0f;
		for (uint32_t queueFamily : uniqueQueueFamilies)
		{
			VkDeviceQueueCreateInfo queueCreateInfo{};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &queuePriority;
			queueCreateInfos.push_back(queueCreateInfo);
		}

		VkPhysicalDeviceFeatures deviceFeatures{};
		deviceFeatures.samplerAnisotropy = VK_TRUE;
		deviceFeatures.sampleRateShading = VK_TRUE;

		VkDeviceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		createInfo.pQueueCreateInfos = queueCreateInfos.data();
		createInfo.pEnabledFeatures = &deviceFeatures;
		createInfo.enabledExtensionCount = static_cast<uint32_t>(m_device.deviceExtensions.size());
		createInfo.ppEnabledExtensionNames = m_device.deviceExtensions.data();
		if (m_enableValidationLayers)
		{
			createInfo.enabledLayerCount = static_cast<uint32_t>(m_validationLayers.size());
			createInfo.ppEnabledLayerNames = m_validationLayers.data();
		}
		else
		{
			createInfo.enabledLayerCount = 0;
		}

		if (vkCreateDevice(m_device.physical, &createInfo, nullptr, &m_device.logical) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create logical device!");
		}

		vkGetDeviceQueue(m_device.logical, indices.graphicsFamily.value(), 0, &m_device.graphicsQueue);
		vkGetDeviceQueue(m_device.logical, indices.presentFamily.value(), 0, &m_device.presentQueue);
	}

	VkSurfaceFormatKHR GraphicsRenderer::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
	{
		for (const auto& availableFormat : availableFormats)
		{
			if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			{
				return availableFormat;
			}
		}

		return availableFormats[0];
	}

	VkPresentModeKHR GraphicsRenderer::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
	{
		for (const auto& availablePresentMode : availablePresentModes)
		{
			if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
			{
				return availablePresentMode;
			}
		}

		return VK_PRESENT_MODE_FIFO_KHR;
	}

	VkExtent2D GraphicsRenderer::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
	{
		if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
		{
			return capabilities.currentExtent;
		}
		else
		{
			int width, height;
			SDL_Vulkan_GetDrawableSize(APP->GetActiveWindow()->Raw(), &width, &height);

			VkExtent2D actualExtent = {
				static_cast<uint32_t>(width),
				static_cast<uint32_t>(height)
			};

			actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
			actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

			return actualExtent;
		}
	}

	void GraphicsRenderer::CreateSwapchain()
	{
		SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(m_device.physical);

		VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);
		VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.presentModes);
		VkExtent2D extent = ChooseSwapExtent(swapChainSupport.capabilities);

		uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
		if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
		{
			imageCount = swapChainSupport.capabilities.maxImageCount;
		}

		VkSwapchainCreateInfoKHR createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = m_surface;
		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = surfaceFormat.format;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.imageExtent = extent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		QueueFamilyIndices indices = FindQueueFamilies(m_device.physical);
		uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };
		if (indices.graphicsFamily != indices.presentFamily)
		{
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = queueFamilyIndices;
		}
		else
		{
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		}
		createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.presentMode = presentMode;
		createInfo.clipped = VK_TRUE;
		createInfo.oldSwapchain = VK_NULL_HANDLE;

		if (vkCreateSwapchainKHR(m_device.logical, &createInfo, NULL, &m_swapchain.sc) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to createswap chain!");
		}

		vkGetSwapchainImagesKHR(m_device.logical, m_swapchain.sc, &imageCount, nullptr);
		m_swapchain.images.resize(imageCount);
		vkGetSwapchainImagesKHR(m_device.logical, m_swapchain.sc, &imageCount, m_swapchain.images.data());

		m_swapchain.imageFormat = surfaceFormat.format;
		m_swapchain.extent = extent;
	}

	void GraphicsRenderer::CreateImageViews()
	{
		m_swapchain.imageViews.resize(m_swapchain.images.size());

		for (size_t i = 0; i < m_swapchain.images.size(); i++)
		{
			m_swapchain.imageViews[i] = CreateImageView(m_swapchain.images[i], m_swapchain.imageFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);
		}
	}

	void GraphicsRenderer::CreateCommandPool()
	{
		QueueFamilyIndices queueFamilyIndices = FindQueueFamilies(m_device.physical);

		VkCommandPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

		if (vkCreateCommandPool(m_device.logical, &poolInfo, nullptr, &m_commandPool) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create command pool!");
		}
	}

	void GraphicsRenderer::LoadModel()
	{
		m_shader = new Shader("shd/triangle.vert", "shd/triangle.frag");

		m_go = new GameObject();
		m_go->AttachComponent(new TransformComponent(m_go));
		m_go->AttachComponent(new RenderComponent(m_go, "res/mdl/viking_room.obj", "res/img/viking_room.png", m_shader));
		TransformComponent* transform = m_go->GetTransformComponent();
		m_shader->AddGameObject(m_go);

		m_cam = new GameObject();
		m_cam->AttachComponent(new TransformComponent(m_cam));
		m_cam->AttachComponent(new CameraComponent(m_cam));
	}

	void GraphicsRenderer::CreateCommandBuffers()
	{
		m_commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = m_commandPool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = (uint32_t)m_commandBuffers.size();

		if (vkAllocateCommandBuffers(m_device.logical, &allocInfo, m_commandBuffers.data()) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to allocate command buffers!");
		}
	}

	VkCommandBuffer GraphicsRenderer::BeginSingleTimeCommands()
	{
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = m_commandPool;
		allocInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;
		vkAllocateCommandBuffers(m_device.logical, &allocInfo, &commandBuffer);

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(commandBuffer, &beginInfo);

		return commandBuffer;
	}

	void GraphicsRenderer::EndSingleTimeCommands(VkCommandBuffer commandBuffer)
	{
		vkEndCommandBuffer(commandBuffer);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		vkQueueSubmit(m_device.graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(m_device.graphicsQueue);

		vkFreeCommandBuffers(m_device.logical, m_commandPool, 1, &commandBuffer);
	}

	void GraphicsRenderer::CreateSyncObjects()
	{
		m_imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		m_renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		m_inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

		VkSemaphoreCreateInfo semaphoreInfo{};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fenceInfo{};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			if (vkCreateSemaphore(m_device.logical, &semaphoreInfo, nullptr, &m_imageAvailableSemaphores[i]) != VK_SUCCESS ||
				vkCreateSemaphore(m_device.logical, &semaphoreInfo, nullptr, &m_renderFinishedSemaphores[i]) != VK_SUCCESS ||
				vkCreateFence(m_device.logical, &fenceInfo, nullptr, &m_inFlightFences[i]) != VK_SUCCESS)
			{
				throw std::runtime_error("Failed to create sync objects!");
			}
		}
	}

	uint32_t GraphicsRenderer::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
	{
		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(m_device.physical, &memProperties);

		for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
		{
			if (typeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
			{
				return i;
			}
		}

		throw std::runtime_error("Failed to find suitable memory type!");
	}

	VkFormat GraphicsRenderer::FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features)
	{
		for (VkFormat format : candidates)
		{
			VkFormatProperties props;
			vkGetPhysicalDeviceFormatProperties(m_device.physical, format, &props);
			if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features)
			{
				return format;
			}
			else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features)
			{
				return format;
			}
		}

		throw std::runtime_error("Failed to find supported format!");
	}

	VkFormat GraphicsRenderer::FindDepthFormat()
	{
		return FindSupportedFormat(
			{ VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT, VK_FORMAT_D32_SFLOAT },
			VK_IMAGE_TILING_OPTIMAL,
			VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
		);
	}

	void GraphicsRenderer::CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
	{
		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = size;
		bufferInfo.usage = usage;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (vkCreateBuffer(m_device.logical, &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create vertex buffer!");
		}

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(m_device.logical, buffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, properties);

		if (vkAllocateMemory(m_device.logical, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to allocate vertex buffer memory!");
		}

		vkBindBufferMemory(m_device.logical, buffer, bufferMemory, 0);
	}

	void GraphicsRenderer::CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
	{
		VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

		VkBufferCopy copyRegion{};
		copyRegion.size = size;
		vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

		EndSingleTimeCommands(commandBuffer);
	}

	void GraphicsRenderer::CreateImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMem, uint32_t mipLevels, VkSampleCountFlagBits numSamples)
	{
		VkImageCreateInfo imageInfo{};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.imageType = VK_IMAGE_TYPE_2D;
		imageInfo.extent.width = width;
		imageInfo.extent.height = height;
		imageInfo.extent.depth = 1;
		imageInfo.mipLevels = mipLevels;
		imageInfo.arrayLayers = 1;
		imageInfo.format = format;
		imageInfo.tiling = tiling;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageInfo.usage = usage;
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageInfo.samples = numSamples;
		imageInfo.flags = 0; // Optional

		if (vkCreateImage(m_device.logical, &imageInfo, nullptr, &image) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create image!");
		}

		VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements(m_device.logical, image, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, properties);

		if (vkAllocateMemory(m_device.logical, &allocInfo, nullptr, &imageMem) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to allocate image memory!");
		}

		vkBindImageMemory(m_device.logical, image, imageMem, 0);
	}

	void GraphicsRenderer::GenerateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels)
	{
		// Check if image format supports linear blitting
		VkFormatProperties formatProperties;
		vkGetPhysicalDeviceFormatProperties(m_device.physical, imageFormat, &formatProperties);
		if (!(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT))
		{
			throw std::runtime_error("Texture image format does not support linear blitting!");
		}

		VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

		VkImageMemoryBarrier barrier{};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.image = image;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;
		barrier.subresourceRange.levelCount = 1;

		int32_t mipWidth = texWidth;
		int32_t mipHeight = texHeight;

		for (uint32_t i = 1; i < mipLevels; i++)
		{
			barrier.subresourceRange.baseMipLevel = i - 1;
			barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

			vkCmdPipelineBarrier(commandBuffer,
								 VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
								 0, nullptr,
								 0, nullptr,
								 1, &barrier);

			VkImageBlit blit{};
			blit.srcOffsets[0] = { 0, 0, 0 };
			blit.srcOffsets[1] = { mipWidth, mipHeight, 1 };
			blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			blit.srcSubresource.mipLevel = i - 1;
			blit.srcSubresource.baseArrayLayer = 0;
			blit.srcSubresource.layerCount = 1;
			blit.dstOffsets[0] = { 0, 0, 0 };
			blit.dstOffsets[1] = { mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1 };
			blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			blit.dstSubresource.mipLevel = i;
			blit.dstSubresource.baseArrayLayer = 0;
			blit.dstSubresource.layerCount = 1;

			vkCmdBlitImage(commandBuffer,
						   image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
						   image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
						   1, &blit,
						   VK_FILTER_LINEAR);

			barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
			barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			vkCmdPipelineBarrier(commandBuffer,
								 VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
								 0, nullptr,
								 0, nullptr,
								 1, &barrier);

			if (mipWidth > 1) mipWidth /= 2;
			if (mipHeight > 1) mipHeight /= 2;
		}

		barrier.subresourceRange.baseMipLevel = mipLevels - 1;
		barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		vkCmdPipelineBarrier(commandBuffer,
							 VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
							 0, nullptr,
							 0, nullptr,
							 1, &barrier);

		EndSingleTimeCommands(commandBuffer);
	}

	void GraphicsRenderer::TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels)
	{
		VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

		VkImageMemoryBarrier barrier{};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.oldLayout = oldLayout;
		barrier.newLayout = newLayout;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.image = image;
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.levelCount = mipLevels;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;

		VkPipelineStageFlags sourceStage;
		VkPipelineStageFlags destinationStage;

		if (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
		{
			barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
			if (HasStencilComponent(format))
			{
				barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
			}
		}

		if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
		{
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		}
		else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
		{
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		}
		else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
		{
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		}
		else
		{
			throw std::invalid_argument("Unsupported layout transition!");
		}

		vkCmdPipelineBarrier(
			commandBuffer,
			sourceStage, destinationStage,
			0,
			0, nullptr,
			0, nullptr,
			1, &barrier
		);

		EndSingleTimeCommands(commandBuffer);
	}

	void GraphicsRenderer::CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height)
	{
		VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

		VkBufferImageCopy region{};
		region.bufferOffset = 0;
		region.bufferRowLength = 0;
		region.bufferImageHeight = 0;

		region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		region.imageSubresource.mipLevel = 0;
		region.imageSubresource.baseArrayLayer = 0;
		region.imageSubresource.layerCount = 1;

		region.imageOffset = { 0, 0, 0 };
		region.imageExtent = {
			width,
			height,
			1
		};

		vkCmdCopyBufferToImage(
			commandBuffer,
			buffer,
			image,
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			1,
			&region
		);

		EndSingleTimeCommands(commandBuffer);
	}

	VkImageView GraphicsRenderer::CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels)
	{
		VkImageViewCreateInfo viewInfo{};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.image = image;
		viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		viewInfo.format = format;
		viewInfo.subresourceRange.aspectMask = aspectFlags;
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = mipLevels;
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = 1;

		VkImageView imageView;
		if (vkCreateImageView(m_device.logical, &viewInfo, nullptr, &imageView) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create texture image view!");
		}

		return imageView;
	}

	void GraphicsRenderer::DrawFrame()
	{
		vkWaitForFences(m_device.logical, 1, &m_inFlightFences[m_currentFrame], VK_TRUE, UINT64_MAX);

		uint32_t imageIndex;
		VkResult result = vkAcquireNextImageKHR(m_device.logical, m_swapchain.sc, UINT64_MAX, m_imageAvailableSemaphores[m_currentFrame], VK_NULL_HANDLE, &imageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			RecreateSwapchain();
			return;
		}
		else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
		{
			throw std::runtime_error("Failed to acquire swapchain image!");
		}

		vkResetFences(m_device.logical, 1, &m_inFlightFences[m_currentFrame]);

		vkResetCommandBuffer(m_commandBuffers[m_currentFrame], 0);

		m_shader->RecordCommandBuffer(m_commandBuffers[m_currentFrame], imageIndex);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[] = { m_imageAvailableSemaphores[m_currentFrame] };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &m_commandBuffers[m_currentFrame];

		VkSemaphore signalSemaphores[] = { m_renderFinishedSemaphores[m_currentFrame] };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		if (vkQueueSubmit(m_device.graphicsQueue, 1, &submitInfo, m_inFlightFences[m_currentFrame]) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to submit draw command buffer!");
		}

		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;

		VkSwapchainKHR swapChains[] = { m_swapchain.sc };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		presentInfo.pImageIndices = &imageIndex;
		presentInfo.pResults = nullptr; // Optional

		result = vkQueuePresentKHR(m_device.presentQueue, &presentInfo);

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_framebufferResized)
		{
			m_framebufferResized = false;
			RecreateSwapchain();
		}
		else if (result != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to present swapchain image!");
		}

		m_currentFrame = (m_currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
	}

	VkSampleCountFlagBits  GraphicsRenderer::GetMaxUsableSampleCount()
	{
		VkPhysicalDeviceProperties physicalDeviceProperties;
		vkGetPhysicalDeviceProperties(m_device.physical, &physicalDeviceProperties);

		VkSampleCountFlags counts = physicalDeviceProperties.limits.framebufferColorSampleCounts & physicalDeviceProperties.limits.framebufferDepthSampleCounts;
		if (counts & VK_SAMPLE_COUNT_64_BIT) { return VK_SAMPLE_COUNT_64_BIT; }
		if (counts & VK_SAMPLE_COUNT_32_BIT) { return VK_SAMPLE_COUNT_32_BIT; }
		if (counts & VK_SAMPLE_COUNT_16_BIT) { return VK_SAMPLE_COUNT_16_BIT; }
		if (counts & VK_SAMPLE_COUNT_8_BIT) { return VK_SAMPLE_COUNT_8_BIT; }
		if (counts & VK_SAMPLE_COUNT_4_BIT) { return VK_SAMPLE_COUNT_4_BIT; }
		if (counts & VK_SAMPLE_COUNT_2_BIT) { return VK_SAMPLE_COUNT_2_BIT; }

		return VK_SAMPLE_COUNT_1_BIT;
	}

	void GraphicsRenderer::WaitForDeviceToIdle()
	{
		vkDeviceWaitIdle(m_device.logical);
	}

	void GraphicsRenderer::RecreateSwapchain()
	{
		while (SDL_GetWindowFlags(APP->GetActiveWindow()->Raw()) & SDL_WINDOW_MINIMIZED)
		{
			SDL_Event e;
			SDL_PollEvent(&e);
		}

		WaitForDeviceToIdle();

		CleanupSwapchain();

		CreateSwapchain();
		CreateImageViews();
		m_shader->SwapchainRecreation();
	}

	void GraphicsRenderer::CleanupSwapchain()
	{
		m_shader->SwapchainCleanup();

		for (auto imageView : m_swapchain.imageViews)
		{
			vkDestroyImageView(m_device.logical, imageView, nullptr);
		}

		vkDestroySwapchainKHR(m_device.logical, m_swapchain.sc, nullptr);
	}

	void GraphicsRenderer::Destroy()
	{
		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			vkDestroySemaphore(m_device.logical, m_renderFinishedSemaphores[i], nullptr);
			vkDestroySemaphore(m_device.logical, m_imageAvailableSemaphores[i], nullptr);
			vkDestroyFence(m_device.logical, m_inFlightFences[i], nullptr);
		}

		m_go->Destroy();
		delete m_go;

		m_shader->Destroy();

		vkDestroyCommandPool(m_device.logical, m_commandPool, nullptr);
		CleanupSwapchain();

		vkDestroyDevice(m_device.logical, nullptr);

		if (m_enableValidationLayers)
		{
			DestroyDebugUtilsMessengerEXT(m_vk, m_debugMessenger, nullptr);
		}

		vkDestroySurfaceKHR(m_vk, m_surface, nullptr);
		vkDestroyInstance(m_vk, nullptr);
	}

	bool GraphicsRenderer::CheckValidationLayerSupport()
	{
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		for (const char* layerName : m_validationLayers)
		{
			bool layerFound = false;
			for (const auto& layerProperties : availableLayers)
			{
				if (std::strcmp(layerName, layerProperties.layerName) == 0)
				{
					layerFound = true;
					break;
				}
			}

			if (!layerFound)
				return false;
		}

		return true;
	}

	std::vector<const char*> GraphicsRenderer::GetRequiredExtensions()
	{
		uint32_t sdlExtensionCount = 0;
		SDL_Vulkan_GetInstanceExtensions(APP->GetActiveWindow()->Raw(), &sdlExtensionCount, nullptr);
		std::vector<const char*> sdlExtensions(sdlExtensionCount);
		SDL_Vulkan_GetInstanceExtensions(APP->GetActiveWindow()->Raw(), &sdlExtensionCount, sdlExtensions.data());

		std::vector<const char*> extensions(sdlExtensions);

		if (m_enableValidationLayers)
		{
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		return extensions;
	}

	void GraphicsRenderer::PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
	{
		createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		createInfo.messageSeverity =
			//VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | //TODO: maybe should activate
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		createInfo.messageType =
			VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		createInfo.pfnUserCallback = DebugCallback;
		createInfo.pUserData = nullptr;
	}

	void GraphicsRenderer::SetupDebugMessenger()
	{
		if (!m_enableValidationLayers)
			return;

		VkDebugUtilsMessengerCreateInfoEXT createInfo;
		PopulateDebugMessengerCreateInfo(createInfo);

		if (CreateDebugUtilsMessengerEXT(m_vk, &createInfo, nullptr, &m_debugMessenger) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to set up debug messenger!");
		}
	}

	void GraphicsRenderer::CreateSurface()
	{
		if (!SDL_Vulkan_CreateSurface(APP->GetActiveWindow()->Raw(), m_vk, &m_surface))
		{
			throw std::runtime_error("Failed to create window surface!");
		}
	}

	QueueFamilyIndices GraphicsRenderer::FindQueueFamilies(VkPhysicalDevice physicalDevice)
	{
		QueueFamilyIndices indices;

		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

		int i = 0;
		for (const auto& queueFamily : queueFamilies)
		{
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				indices.graphicsFamily = i;
			}

			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, m_surface, &presentSupport);
			if (presentSupport)
			{
				indices.presentFamily = i;
			}

			if (indices.IsComplete())
				break;

			i++;
		}

		return indices;
	}

	SwapChainSupportDetails GraphicsRenderer::QuerySwapChainSupport(VkPhysicalDevice physicalDevice)
	{
		SwapChainSupportDetails details;

		//capabilities
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, m_surface, &details.capabilities);

		//formats
		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, m_surface, &formatCount, nullptr);
		if (formatCount != 0)
		{
			details.formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, m_surface, &formatCount, details.formats.data());
		}

		//present modes
		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, m_surface, &presentModeCount, nullptr);
		if (presentModeCount != 0)
		{
			details.presentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, m_surface, &presentModeCount, details.presentModes.data());
		}

		return details;
	}

	// validation callback
	VKAPI_ATTR VkBool32 VKAPI_CALL GraphicsRenderer::DebugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData)
	{
		if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
		{
			std::cerr << "Validation layer ERROR: " << pCallbackData->pMessage << "\n\n";
		}
		else if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
		{
			std::cout << "Validation layer WARNING: " << pCallbackData->pMessage << "\n\n";
		}

		return VK_FALSE;
	}

	VkResult CreateDebugUtilsMessengerEXT(
		VkInstance instance,
		const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkDebugUtilsMessengerEXT* pDebugMessenger)
	{
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
		if (func != nullptr)
		{
			return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
		}
		else
		{
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}
	}

	void DestroyDebugUtilsMessengerEXT(
		VkInstance instance, 
		VkDebugUtilsMessengerEXT debugMessenger, 
		const VkAllocationCallbacks* pAllocator)
	{
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr)
		{
			func(instance, debugMessenger, pAllocator);
		}
	}
}




