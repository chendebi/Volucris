#include "Renderer/primitive_proxy.h"
#include "Scene/primitive_component.h"
#include "Resource/mesh_resource_data.h"
#include "Core/assert.h"
#include "Renderer/material_proxy.h"
#include "Resource/material.h"

namespace volucris
{
	PrimitiveProxy::PrimitiveProxy(PrimitiveComponent* primitive)
		: m_renderData(primitive->getMeshResourceData()->build())
	{
		auto meshData = primitive->getMeshResourceData();
		std::vector<SectionRenderData> sections;
		for (const auto& section : meshData->getSections())
		{
			SectionRenderData sectionRenderData;
			sectionRenderData.mode = section.mode;
			sectionRenderData.count = section.count;
			sectionRenderData.offset = section.offset;
			auto mat = meshData->getMaterial(section.slot);

			check(mat != nullptr);
			sectionRenderData.material = mat->attachProxy();
			sections.push_back(sectionRenderData);
		}
	}
}
