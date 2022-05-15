#pragma once

#include "Renderer.h"

namespace abc
{
	class Pipeline
	{
	public:
		Pipeline(Renderer* renderer);
		~Pipeline();

		void Destroy();

	protected:
		Renderer* m_renderer{};

		std::vector<char> m_vertCode;
		std::vector<char> m_fragCode;
		VkShaderModule m_vertModule;
		VkShaderModule m_fragModule;
		VkPipelineLayout m_pipelineLayout;
		VkPipeline m_pipeline;

		VkShaderModule CreateShaderModule(const std::vector<char>& code);

		friend class Renderer;
	};
}

