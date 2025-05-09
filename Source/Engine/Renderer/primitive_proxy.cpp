#include "Renderer/primitive_proxy.h"
#include "Scene/primitive_component.h"
#include "Resource/mesh_resource_data.h"

namespace volucris
{
	PrimitiveProxy::PrimitiveProxy(PrimitiveComponent* primitive)
		: m_renderData(primitive->getMeshResourceData()->build())
	{
		auto meshData = primitive->getMeshResourceData();
		for (const auto& section : meshData->getSections())
		{
			if (const auto material = meshData->getMaterial(section->slot))
			{
				//material-
			}
			else
			{
				// 使用默认材质?
			}
		}
	}
}
