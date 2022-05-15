#include "pch.h"
#include "Framebuffer.h"

#include "Swapchain.h"
#include "Device.h"
#include "RenderPass.h"

namespace abc
{
	Framebuffer::Framebuffer(Renderer* renderer, int index) :
		m_renderer(renderer)
	{
        VkImageView attachments[] = {
        m_renderer->GetSwapchain()->GetImageViews()[index]
        };

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = m_renderer->GetRenderPass()->GetRenderPass();
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = m_renderer->GetSwapchain()->GetExtent().width;
        framebufferInfo.height = m_renderer->GetSwapchain()->GetExtent().height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(m_renderer->GetDevice()->GetLogical(), &framebufferInfo, nullptr, &m_framebuffer) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create framebuffer!");
        }
	}

	Framebuffer::~Framebuffer()
	{
	}

	void Framebuffer::Destroy()
	{
        vkDestroyFramebuffer(m_renderer->GetDevice()->GetLogical(), m_framebuffer, nullptr);
	}
}