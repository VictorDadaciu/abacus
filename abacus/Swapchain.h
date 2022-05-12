#pragma once

#include "Renderer.h"

namespace abc
{
	class Swapchain
	{
	public:
		Swapchain(Renderer* renderer);
		~Swapchain();

		void Destroy();

	protected:
		Renderer* m_renderer{};

		VkSwapchainKHR m_swapchain{};
		std::vector<VkImage> m_swapchainImages{};
		std::vector<VkImageView> m_swapchainImageViews;
		VkFormat m_swapchainImageFormat{};
		VkExtent2D m_swapchainExtent;

		VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
		VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
		void CreateSwapChain();
		void CreateImageViews();

		friend class Renderer;
	};
}

