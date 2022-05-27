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

	void Buffer::Destroy()
	{
		vkDestroyBuffer(RENDERER->GetDevice().logical, m_data, nullptr);
		vkFreeMemory(RENDERER->GetDevice().logical, m_mem, nullptr);
	}

	VertexBuffer::VertexBuffer(const std::vector<Vertex>& vertices)
	{
		m_type = BufferType::VERTEX;

		VkBuffer stagingBuffer{};
		VkDeviceMemory stagingMemory{};

		VkDeviceSize bufferSize;
		bufferSize = sizeof(vertices[0]) * vertices.size();

		RENDERER->CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingMemory);

		void* data;
		vkMapMemory(RENDERER->GetDevice().logical, stagingMemory, 0, bufferSize, 0, &data);
		memcpy(data, vertices.data(), (size_t)bufferSize);
		vkUnmapMemory(RENDERER->GetDevice().logical, stagingMemory);

		RENDERER->CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_data, m_mem);
		RENDERER->CopyBuffer(stagingBuffer, m_data, bufferSize);

		vkDestroyBuffer(RENDERER->GetDevice().logical, stagingBuffer, nullptr);
		vkFreeMemory(RENDERER->GetDevice().logical, stagingMemory, nullptr);
	}

	VertexBuffer::~VertexBuffer()
	{
	} 

	IndexBuffer::IndexBuffer(const std::vector<uint32_t>& indices)
	{
		m_type = BufferType::INDEX;
		 
		VkBuffer stagingBuffer{};   
		VkDeviceMemory stagingMemory{}; 

		VkDeviceSize bufferSize; 
		bufferSize = sizeof(indices[0]) * indices.size();    

		RENDERER->CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingMemory);
		  
		void* data;
		vkMapMemory(RENDERER->GetDevice().logical, stagingMemory, 0, bufferSize, 0, &data);
		memcpy(data, indices.data(), (size_t)bufferSize);
		vkUnmapMemory(RENDERER->GetDevice().logical, stagingMemory);
		 
		RENDERER->CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_data, m_mem);
		RENDERER->CopyBuffer(stagingBuffer, m_data, bufferSize); 

		vkDestroyBuffer(RENDERER->GetDevice().logical, stagingBuffer, nullptr);
		vkFreeMemory(RENDERER->GetDevice().logical, stagingMemory, nullptr);
	}
 
	IndexBuffer::~IndexBuffer()
	{
	}

	UniformBuffer::UniformBuffer(VkDeviceSize size)
	{ 
		m_type = BufferType::UNIFORM;   

		RENDERER->CreateBuffer(size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_data, m_mem);
	}

	UniformBuffer::~UniformBuffer()
	{ 
	}
	 
	void UniformBuffer::UpdateMemory(void* ubo, VkDeviceSize size)
	{  
		void* data;
		vkMapMemory(RENDERER->GetDevice().logical, m_mem, 0, size, 0, &data);   
		memcpy(data, ubo, size);    
		vkUnmapMemory(RENDERER->GetDevice().logical, m_mem);  
	} 
} 