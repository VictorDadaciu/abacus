#pragma once

#include "Renderer.h"

namespace abc
{
	class RenderPass
	{
	public:
		RenderPass(Renderer* renderer);
		~RenderPass();

		const VkRenderPass GetRenderPass() const { return m_renderPass; }

		void Destroy();

	protected:
		Renderer* m_renderer;

		VkRenderPass m_renderPass;

		friend class Renderer;
	};
}

