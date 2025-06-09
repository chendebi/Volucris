#include "Renderer/primitive_proxy.h"
#include "Scene/primitive_component.h"
#include "Resource/mesh_resource_data.h"
#include "Core/assert.h"
#include "Renderer/material_proxy.h"
#include "Resource/material.h"
#include "Renderer/OpenGL/ogl_buffer_object.h"
#include "Renderer/OpenGL/ogl_vertex_array_object.h"
#include "Resource/mesh_resource.h"
#include "Renderer/mesh_proxy.h"

namespace volucris
{
	PrimitiveProxy::PrimitiveProxy()
		: m_meshProxy(nullptr)
		, m_batches()
	{
		
	}

	void PrimitiveProxy::initialize(PrimitiveComponent* primitive)
	{
		auto resource = primitive->getResource();
		m_meshProxy = resource->getRenderProxy();
		std::unordered_map<std::shared_ptr<MaterialProxy>, std::vector<SectionRenderData>> sections;
		for (const auto& section : resource->getResourceData()->getSections())
		{
			SectionRenderData sectionRenderData;
			sectionRenderData.mode = section.mode;
			sectionRenderData.count = section.count;
			sectionRenderData.offset = section.offset * sizeof(uint32);
			auto mat = primitive->getMaterial(section.slot);

			check(mat != nullptr);

			auto matProxy = mat->getRenderProxy();
			auto sectionIt = sections.find(matProxy);
			if (sectionIt == sections.end())
			{
				sections[matProxy] = { sectionRenderData };
			}
			else
			{
				sectionIt->second.push_back(sectionRenderData);
			}
		}

		m_sections = sections;
		m_batches.reserve(m_sections.size());
		for (auto& [material, sections] : m_sections)
		{
			PrimitiveDrawBatch batch;
			batch.material = material.get();
			for (auto& section : sections)
			{
				SectionDrawData drawData;
				drawData.section = &section;
				drawData.renderInfo = m_meshProxy->getRenderInfo();
				batch.sections.push_back(drawData);
			}
			m_batches.push_back(batch);
		}
	}
}
