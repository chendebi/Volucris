#include "Resource/mesh_resource_data.h"

namespace volucris
{
	Section::Section(void* data, size_t size)
		: Section()
	{
		auto count = size / sizeof(uint32);
		indices.resize(count);
		memcpy(indices.data(), data, size);
	}

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

	std::shared_ptr<MeshRenderData> MeshResourceData::build()
	{
		auto vertexBufferSize = m_vertices.size() * sizeof(glm::vec3);
		std::vector<uint8> vertexBufferData;
		vertexBufferData.resize(vertexBufferSize);
		memcpy(vertexBufferData.data(), m_vertices.data(), vertexBufferSize);

		std::vector<uint8> indexBufferData;
		std::vector<SectionRenderData> sectionRenderDatas;
		auto indexBufferSize = 0;
		for (const auto& section : m_sections)
		{
			SectionRenderData sectionRenderData;
			sectionRenderData.mode = section->mode;
			sectionRenderData.count = section->indices.size();
			sectionRenderData.offset = indexBufferSize;
			indexBufferSize += (sizeof(uint32) * sectionRenderData.count);
			sectionRenderDatas.push_back(sectionRenderData);
		}

		indexBufferData.resize(indexBufferSize);
		auto offset = 0;
		for (const auto& section : m_sections)
		{
			auto size = section->indices.size() * sizeof(uint32);
			memcpy(indexBufferData.data() + offset, section->indices.data(), size);
			offset += size;
		}

		auto renderData = std::make_shared<MeshRenderData>();
		renderData->m_renderData = std::move(vertexBufferData);
		renderData->m_sectionData = std::move(indexBufferData);
		renderData->m_sections = std::move(sectionRenderDatas);
		return renderData;
	}
}