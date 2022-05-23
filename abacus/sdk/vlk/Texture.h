#pragma once

#include "GraphicsRenderer.h"

namespace abc
{
	class Texture
	{
	public:
		Texture(const std::string& texturePath);
		~Texture();

		const VkImage& GetImage() const { return m_image; }
		const VkImageView& GetImageView() const { return m_view; }
		const VkDeviceMemory& GetImageMemory() const { return m_mem; }
		const VkSampler& GetSampler() const { return m_sampler; }

		void Destroy();

	protected:
		VkImage m_image{};
		VkDeviceMemory m_mem{};
		VkImageView m_view{};
		VkSampler m_sampler{};
		uint32_t m_mipLevels{};

		void CreateImage(const std::string& texurePath);
		void CreateImageView();
		void CreateSampler();
	};
}

