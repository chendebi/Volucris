#include "Renderer/primitive_proxy.h"
#include "Scene/primitive_component.h"
#include "Resource/mesh_resource_data.h"
#include "Core/assert.h"
#include "Renderer/material_proxy.h"
#include "Resource/material.h"

namespace volucris
{
	PrimitiveProxy::PrimitiveProxy(PrimitiveComponent* primitive)
		: ProxyObject()
		, m_renderData(primitive->getMeshResourceData()->build())
	{
		primitive->setProxy(this);
		setSceneObject(primitive);
		auto meshData = primitive->getMeshResourceData();
		std::vector<SectionRenderData> sections;
		for (const auto& section : meshData->getSections())
		{
			SectionRenderData sectionRenderData;
			sectionRenderData.mode = section.mode;
			sectionRenderData.count = section.count;
			sectionRenderData.offset = section.offset;
			std::shared_ptr<Material> mat = nullptr;
			auto mat = meshData->getMaterial(section.slot);

			check(mat != nullptr);
			if (auto proxy = mat->getProxy())
			{
				std::make_shared(this);
			}

			sectionRenderData.material = std::make_shared<MaterialProxy>(mat.get());
			sections.push_back(sectionRenderData);
		}
	}
}
