#pragma once

#include <vulkan.h>

namespace abc
{
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
		bool operator==(const Vertex& other) const { return pos == other.pos && color == other.color && uv == other.uv; }
	};

	class GameObject;
	class Shader
	{
	public:
		Shader(const std::string& vertPath, const std::string& fragPath);
		~Shader();

		const Pipeline& GetPipeline() const { return m_pipeline; }
		const VkDescriptorSetLayout& GetDescriptorSetLayout() const { return m_descriptorSetLayout; }
		const VkDescriptorPool& GetDescriptorPool() const { return m_descriptorPool; }
		const VkRenderPass& GetRenderPass() const { return m_renderPass; }

		void RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

		void AddGameObject(GameObject* go);

		void Destroy();
		void SwapchainCleanup();
		void SwapchainRecreation();

	protected:
		Pipeline m_pipeline{};
		VkDescriptorSetLayout m_descriptorSetLayout{};
		VkDescriptorPool m_descriptorPool{};
		VkRenderPass m_renderPass{};
		std::vector<VkFramebuffer> m_framebuffers{};
		std::vector<std::vector<VkCommandBuffer>> m_secondaryCommandBuffers{};
		VkImage m_depthImage{};
		VkDeviceMemory m_depthImageMem{};
		VkImageView m_depthImageView{};
		VkImage m_colorImage{};
		VkDeviceMemory m_colorImageMem{};
		VkImageView m_colorImageView{};

		std::vector<GameObject*> m_gameObjects{};

		void CreateRenderPass();
		void CreateDescriptorSetLayout();
		void CreatePipeline();
		VkShaderModule CreateShaderModule(const std::vector<char>& code);
		void CreateFramebuffers();
		void CreateDescriptorPool();
		void CreateColorResources();
		void CreateDepthResources();

		void ResizeSecondaryCommandBufferMatrix(uint32_t imageIndex);

		void RecordSecondaryCommandBuffers(const VkCommandBuffer& commandBuffer, uint32_t imageIndex);
	};
}

