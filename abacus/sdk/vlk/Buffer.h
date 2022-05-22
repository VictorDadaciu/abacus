#pragma once

#include "GraphicsRenderer.h"
#include "Shader.h"

#include "glm.hpp"

namespace abc
{
	enum class BufferType
	{
		NONE = 0,
		VERTEX,
		INDEX,
		UNIFORM
	};

	class Buffer
	{
	public:
		Buffer() = default;
		Buffer(const VkBuffer& data, const VkDeviceMemory& mem, BufferType type);
		~Buffer();

		const BufferType& GetBufferType() const { return m_type; }
		const VkBuffer& GetBuffer() const { return m_data; }
		const VkDeviceMemory& GetBufferMemory() const { return m_mem; }

		void Destroy();

	protected:
		VkBuffer m_data{};
		VkDeviceMemory m_mem{};
		BufferType m_type{};
	};

	class VertexBuffer : public Buffer
	{
	public:
		VertexBuffer(const std::vector<Vertex>& vertices);
		~VertexBuffer();
	};

	class IndexBuffer : public Buffer
	{
	public:
		IndexBuffer(const std::vector<uint32_t>& indices);
		~IndexBuffer();
	};

	class UniformBuffer : public Buffer
	{
	public:
		UniformBuffer();
		~UniformBuffer();
	};
}

