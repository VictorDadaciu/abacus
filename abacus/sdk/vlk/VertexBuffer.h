#pragma once

#include "Renderer.h"

#include "glm.hpp"

namespace abc
{
	struct Vertex
	{
		glm::vec2 pos;
		glm::vec3 color;

		static VkVertexInputBindingDescription GetBindingDescription();
		static const std::array<VkVertexInputAttributeDescription, 2>& GetAttributeDescriptions();
	};

	enum class BufferType
	{
		VERTEX,
		INDEX
	};

	class VertexBuffer
	{
	public:
		VertexBuffer(VkCommandPool commandPool, Device device, BufferType type);
		~VertexBuffer();

		void Destroy(VkDevice device);

	protected:
		const std::vector<Vertex> vertices = {
			{{-0.5f, -0.5f}, {0.98f, 0.1f, 0.1f}},
			{{0.5f, -0.5f}, {0.1f, 0.98f, 0.1f}},
			{{0.5f, 0.5f}, {0.1f, 0.1f, 0.98f}},
			{{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
		};

		const std::vector<uint16_t> indices = {
			0, 1, 2, 2, 3, 0
		};

		VkBuffer dataBuffer{};
		VkDeviceMemory dataMemory{};
		BufferType type;

		uint32_t FindMemoryType(Device device, uint32_t typeFilter, VkMemoryPropertyFlags properties);
		void CreateBuffer(Device device, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
		void CopyBuffer(VkCommandPool commandPool, Device device, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

		friend class Renderer;
	};
}

