#include "pch.h"
#include "Buffer.h"

namespace abc
{
	Buffer::Buffer(const VkBuffer& data, const VkDeviceMemory& mem, BufferType type) :
		m_data(data), m_mem(mem), m_type(type)
	{
	}

	Buffer::~Buffer()
	{
	}

	void Buffer::Destroy(VkDevice device)
	{
		vkDestroyBuffer(device, m_data, nullptr);
		vkFreeMemory(device, m_mem, nullptr);
	}
}