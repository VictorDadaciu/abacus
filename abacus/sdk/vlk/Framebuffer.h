#pragma once

#include "Renderer.h"

namespace abc
{
	class Framebuffer
	{
	public:
		Framebuffer(Renderer* renderer, int index);
		~Framebuffer();

		void Destroy();

	protected:
		Renderer* m_renderer;
		VkFramebuffer m_framebuffer;

		friend class Renderer;
	};
}

