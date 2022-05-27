#pragma once

#include <vulkan.h>
#include "Buffer.h"
#include "Shader.h"

namespace abc
{
	struct Vertex;
	class Model
	{
	public:
		Model(const std::string& meshPath, Shader* shader);

		const std::vector<Vertex>& GetVertices() const { return m_vertices; }
		const std::vector<uint32_t>& GetIndices() const { return m_indices; }
		VertexBuffer* GetVertexBuffer() const { return m_vertexBuffer; }
		const VkBuffer& GetVertexBufferRaw() const { return m_vertexBuffer->GetBuffer(); }
		IndexBuffer* GetIndexBuffer() const { return m_indexBuffer; }
		const VkBuffer& GetIndexBufferRaw() const { return m_indexBuffer->GetBuffer(); }
		UniformBuffer* GetUniformBuffer(int index) const { return m_uniformBuffers[index]; }
		const VkBuffer& GetUniformBufferRaw(int index) const { return m_uniformBuffers[index]->GetBuffer(); }
		const VkDescriptorSet& GetDescriptorSet(int index) const { return m_descriptorSets[index]; }
		const Shader* GetShader() const { return m_shader; }

		void Destroy();

		~Model();

	protected:
		std::vector<Vertex> m_vertices{};
		std::vector<uint32_t> m_indices{};
		VertexBuffer* m_vertexBuffer{};
		IndexBuffer* m_indexBuffer{};
		std::vector<UniformBuffer*> m_uniformBuffers{};
		std::vector<VkDescriptorSet> m_descriptorSets{};
		Shader* m_shader{};

		void CreateDescriptorSets();
		void CreateUniformBuffers();
	};
}

