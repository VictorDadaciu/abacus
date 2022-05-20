#pragma once

#include "GraphicsRenderer.h"
#include "Buffer.h"

namespace abc
{
	class Model
	{
	public:
		Model(const std::string& meshPath);

		const std::vector<Vertex>& GetVertices() const { return m_vertices; }
		const std::vector<uint32_t>& GetIndices() const { return m_indices; }
		const Buffer* GetVertexBuffer() const { return m_vertexBuffer; }
		const VkBuffer& GetVertexBufferRaw() const { return m_vertexBuffer->GetBuffer(); }
		const Buffer* GetIndexBuffer() const { return m_indexBuffer; }
		const VkBuffer& GetIndexBufferRaw() const { return m_indexBuffer->GetBuffer(); }

		void Destroy();

		~Model();

	protected:
		std::vector<Vertex> m_vertices{};
		std::vector<uint32_t> m_indices{};
		Buffer* m_vertexBuffer{};
		Buffer* m_indexBuffer{};
	};
}

