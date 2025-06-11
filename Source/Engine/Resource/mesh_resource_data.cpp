#include "Resource/mesh_resource_data.h"
#include "Resource/material.h"
#include "Core/assert.h"
#include <Core/volucris.h>

namespace volucris
{
	MeshResourceData::MeshResourceData()
		: m_vertices()
		, m_normals()
		, m_uvs({ {}, {} })
		, m_sections()
	{
	}

	void MeshResourceData::setVertices(void* vertices, size_t size)
	{
		auto count = size / sizeof(glm::vec3);
		m_vertices.resize(count);
		memcpy(m_vertices.data(), vertices, size);
	}

	void MeshResourceData::setVertices(std::vector<glm::vec3> vertices)
	{
		m_vertices = std::move(vertices);
	}

	void MeshResourceData::setNormals(std::vector<glm::vec3> normals)
	{
		m_normals = std::move(normals);
	}

	void MeshResourceData::setUV(int idx, std::vector<glm::vec3> uv)
	{
		if (idx == 0 || idx == 1)
		{
			m_uvs[idx] = std::move(uv);
		}
	}

	void MeshResourceData::addSection(const std::vector<uint32>& indices, const std::string& slot, DrawMode mode)
	{
		auto section = addSectionData(indices);
		section.mode = mode;
		section.slot = slot;
		addSection(section);
	}

	void MeshResourceData::addSection(const Section& section)
	{
		m_sections.push_back(section);
	}

	Section MeshResourceData::addSectionData(const std::vector<uint32>& indices)
	{
		return addSectionData(indices.data(), indices.size());
	}

	Section MeshResourceData::setSectionData(std::vector<uint32> indices)
	{
		m_indices = std::move(indices);
		Section section;
		section.count = m_indices.size();
		section.offset = 0;
		return section;
	}

	Section MeshResourceData::addSectionData(const uint32* indices, size_t count)
	{
		size_t currentCount = m_indices.size();
		size_t newcount = currentCount + count;
		size_t offset = currentCount * sizeof(uint32);
		m_indices.resize(newcount);
		memcpy(m_indices.data() + currentCount, indices, count * sizeof(uint32));
		Section section;
		section.count = count;
		section.offset = currentCount * sizeof(uint32);
		return section;
	}

	std::shared_ptr<MeshRenderData> MeshResourceData::build()
	{
		if (m_vertices.empty() || m_indices.empty())
		{
			V_LOG_WARN(Engine, "invalid mesh resource. build failed.");
			return nullptr;
		}

		auto renderData = std::make_shared<MeshRenderData>();
		auto vertexBufferSize = m_vertices.size() * sizeof(glm::vec3);
		auto normalBufferSize = m_normals.size() * sizeof(glm::vec3);
		auto uv0BufferSize = m_uvs[0].size() * sizeof(glm::vec3);
		auto uv1BufferSize = m_uvs[1].size() * sizeof(glm::vec3);

		size_t offset = 0;
		std::vector<uint8> vertexBufferData;
		vertexBufferData.resize(vertexBufferSize + normalBufferSize + uv0BufferSize + uv1BufferSize);
		memcpy(vertexBufferData.data(), m_vertices.data(), vertexBufferSize);
		{
			BlockDescription vertexBlock = { BlockType::VERTEX, offset };
			renderData->blocks.push_back(vertexBlock);
			offset += vertexBufferSize;
		}

		if (normalBufferSize > 0)
		{
			memcpy(vertexBufferData.data() + offset, m_normals.data(), normalBufferSize);
			BlockDescription block = { BlockType::NORMAL, offset };
			renderData->blocks.push_back(block);
			offset += normalBufferSize;
		}

		if (uv0BufferSize > 0)
		{
			memcpy(vertexBufferData.data() + offset, m_uvs[0].data(), uv0BufferSize);
			BlockDescription block = { BlockType::UV0, offset };
			renderData->blocks.push_back(block);
			offset += uv0BufferSize;
		}

		if (uv1BufferSize > 0)
		{
			memcpy(vertexBufferData.data() + offset, m_uvs[1].data(), uv1BufferSize);
			BlockDescription block = { BlockType::UV1, offset };
			renderData->blocks.push_back(block);
			offset += uv1BufferSize;
		}

		std::vector<uint8> indexBufferData;
		indexBufferData.resize(sizeof(uint32) * m_indices.size());
		memcpy(indexBufferData.data(), m_indices.data(), indexBufferData.size());

		renderData->renderData = std::move(vertexBufferData);
		renderData->sectionData = std::move(indexBufferData);
		return renderData;
	}

	void MeshResourceData::serialize(Serializer& serializer)
	{
		serializer.serialize(m_vertices);
		serializer.serialize(m_normals);
		serializer.serialize(m_uvs[0]);
		serializer.serialize(m_uvs[1]);
		serializer.serialize(m_indices);
		serializer.serialize(m_sections);
	}

	void MeshResourceData::deserialize(Serializer& serializer)
	{
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec3> uv0;
		std::vector<glm::vec3> uv1;
		std::vector<uint32> indices;
		std::vector<Section> sections;

		if (serializer.deserialize(vertices) && 
			serializer.deserialize(normals) &&
			serializer.deserialize(uv0) &&
			serializer.deserialize(uv1) &&
			serializer.deserialize(indices) &&
			serializer.deserialize(sections)
			)
		{
			m_vertices = std::move(vertices);
			m_normals = std::move(normals);
			m_uvs[0] = std::move(uv0);
			m_uvs[1] = std::move(uv1);
			m_indices = std::move(indices);
			m_sections = std::move(sections);
		}
		else
		{
			V_LOG_WARN(Engine, "deserialize MeshResourceData failed");
		}
	}
}