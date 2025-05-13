#include "Resource/mesh_resource_data.h"
#include "Resource/material.h"
#include "Core/assert.h"

namespace volucris
{
	MeshResourceData::MeshResourceData()
		: m_vertices()
		, m_sections()
		, m_materials()
	{
	}

	void MeshResourceData::setVertices(void* vertices, size_t size)
	{
		auto count = size / sizeof(glm::vec3);
		m_vertices.resize(count);
		memcpy(m_vertices.data(), vertices, size);
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
		return addSectionData((void*)indices.data(), (size_t)indices.size() * sizeof(uint32));
	}

	Section MeshResourceData::addSectionData(void* indices, size_t size)
	{
		size_t appendCount = size / sizeof(uint32);
		size_t currentCount = m_indices.size();
		size_t count = currentCount + appendCount;
		size_t offset = currentCount * sizeof(uint32);
		m_indices.resize(count);
		memcpy(m_indices.data() + offset, indices, size);
		Section data;
		data.count = appendCount;
		data.offset = offset;
		return data;
	}

	std::shared_ptr<MeshRenderData> MeshResourceData::build()
	{
		auto vertexBufferSize = m_vertices.size() * sizeof(glm::vec3);
		std::vector<uint8> vertexBufferData;
		vertexBufferData.resize(vertexBufferSize);
		memcpy(vertexBufferData.data(), m_vertices.data(), vertexBufferSize);

		std::vector<uint8> indexBufferData;
		indexBufferData.resize(sizeof(uint32) * m_indices.size());
		memcpy(indexBufferData.data(), m_indices.data(), indexBufferData.size());

		auto renderData = std::make_shared<MeshRenderData>();
		renderData->renderData = std::move(vertexBufferData);
		renderData->sectionData = std::move(indexBufferData);
		return renderData;
	}
}