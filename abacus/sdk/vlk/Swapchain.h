#pragma once

#include "Renderer.h"

namespace abc
{
	class Swapchain
	{
	public:
		Swapchain(Renderer* renderer);
		~Swapchain();

		const VkFormat GetFormat() const { return m_imageFormat; }
		const VkExtent2D GetExtent() const { return m_extent; };
		const std::vector<VkImageView>& GetImageViews() const { return m_imageViews; }
		const std::vector<VkImage>& GetImages() const { return m_images; }

		void Destroy();

	protected:
		Renderer* m_renderer{};

		VkSwapchainKHR m_swapchain{};
		std::vector<VkImage> m_images{};
		std::vector<VkImageView> m_imageViews;
		VkFormat m_imageFormat{};
		VkExtent2D m_extent;

		VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
		VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
		void CreateSwapChain();
		void CreateImageViews();

		friend class Renderer;
	};
}

