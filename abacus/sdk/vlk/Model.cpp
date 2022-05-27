#include "pch.h"
#include "Model.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include "Buffer.h"
#include "Shader.h"
#include "GraphicsRenderer.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <gtx/hash.hpp>
namespace std
{
    template<>
    struct hash<abc::Vertex>
    {
        size_t operator()(abc::Vertex const& vertex) const
        {
            return (((hash<glm::vec3>()(vertex.pos) ^ (hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^ (hash<glm::vec3>()(vertex.norm) << 1) >> 1) ^ (hash<glm::vec2>()(vertex.uv) << 1);
        }
    };
}

namespace abc
{
	Model::Model(const std::string& meshPath, Shader* shader) :
		m_shader(shader)
	{
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;

        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, meshPath.c_str()))
        {
            throw std::runtime_error(warn + err);
        }

        std::unordered_map<Vertex, uint32_t> uniqueVertices{};

        for (const auto& shape : shapes)
        {
            for (const auto& index : shape.mesh.indices)
            {
                Vertex vertex{};

                vertex.pos = {
                    attrib.vertices[3 * index.vertex_index + 0],
                    attrib.vertices[3 * index.vertex_index + 1],
                    attrib.vertices[3 * index.vertex_index + 2],
                };

                vertex.norm = {
                    attrib.normals[3 * index.normal_index + 0],
                    attrib.normals[3 * index.normal_index + 1],
                    attrib.normals[3 * index.normal_index + 2],
                };
                
                vertex.uv = {
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    1.0f - attrib.texcoords[2 * index.texcoord_index + 1],
                };

                vertex.color = { 1.0f, 1.0f, 1.0f };

                if (uniqueVertices.count(vertex) == 0)
                {
                    uniqueVertices[vertex] = static_cast<uint32_t>(m_vertices.size());
                    m_vertices.push_back(vertex);
                }

                m_indices.push_back(uniqueVertices[vertex]);
            }
        }

        m_vertexBuffer = new VertexBuffer(m_vertices);
        m_indexBuffer = new IndexBuffer(m_indices);
        CreateUniformBuffers();
        CreateDescriptorSets();
	}

    void Model::Destroy()
    {
        m_vertexBuffer->Destroy();
        delete m_vertexBuffer;

        m_indexBuffer->Destroy();
        delete m_indexBuffer;

        for (size_t i = 0; i < m_uniformBuffers.size(); i++)
        {
            m_uniformBuffers[i]->Destroy();
            delete m_uniformBuffers[i];
        }
    }

    Model::~Model()
    {
    }
     
	void Model::CreateUniformBuffers()
	{
		m_uniformBuffers.resize(RENDERER->MAX_FRAMES_IN_FLIGHT);
		for (size_t i = 0; i < m_uniformBuffers.size(); i++)
		{
			m_uniformBuffers[i] = new UniformBuffer(sizeof(UniformBufferObject)); 
		}
	}

	void Model::CreateDescriptorSets()
	{
		std::vector<VkDescriptorSetLayout> layouts(RENDERER->MAX_FRAMES_IN_FLIGHT, m_shader->GetDescriptorSetLayout());
		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = m_shader->GetDescriptorPool();
		allocInfo.descriptorSetCount = static_cast<uint32_t>(RENDERER->MAX_FRAMES_IN_FLIGHT);
		allocInfo.pSetLayouts = layouts.data();

		m_descriptorSets.resize(RENDERER->MAX_FRAMES_IN_FLIGHT);
		if (vkAllocateDescriptorSets(RENDERER->GetDevice().logical, &allocInfo, m_descriptorSets.data()) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to allocate descriptor sets!"); 
		}
	}
}