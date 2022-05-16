#pragma once

#include "Renderer.h"

#include "glm.hpp"

namespace abc
{
	enum class BufferType
	{
		VERTEX,
		INDEX
	};

	class Buffer
	{
	public:
		Buffer(const VkBuffer& data, const VkDeviceMemory& mem, BufferType type);
		~Buffer();

		const BufferType& GetBufferType() const { return m_type; }
		const VkBuffer& GetBuffer() const { return m_data; }
		const VkDeviceMemory& GetBufferMemory() const { return m_mem; }

		void Destroy(VkDevice device);

	protected:
		VkBuffer m_data{};
		VkDeviceMemory m_mem{};
		BufferType m_type;
	};
}

