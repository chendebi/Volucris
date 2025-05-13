#include "Scene/primitive_component.h"
#include "Core/volucris.h"
#include <Core/assert.h>
#include <Application/application.h>
#include <Renderer/renderer.h>
#include "Scene/scene.h"
#include "Engine/Resource/mesh_resource_data.h"
#include "Resource/material.h"

namespace volucris
{
	PrimitiveComponent::PrimitiveComponent()
		: SceneComponent()
		, m_proxy(nullptr)
		, m_meshData(nullptr)
	{
	}

	void PrimitiveComponent::setResourceData(const std::shared_ptr<MeshResourceData>& data)
	{
		m_meshData = data;
		markRenderStateDirty();
	}

	MeshResourceData* PrimitiveComponent::getMeshResourceData()
	{
		if (!m_meshData)
		{
			m_meshData = std::make_shared<MeshResourceData>();
		}
		return m_meshData.get();
	}


	void PrimitiveComponent::updateRenderState()
	{
		auto renderer = gApp->getRenderer();
		if (!renderer || !getScene()->getSceneProxy())
		{
			m_proxy = nullptr;
			return;
		}

		if (m_proxy)
		{
			renderer->pushCommand([this]() {
				//delete m_proxy
				});
		}

		if (isAttached() && m_meshData)
		{
			// 创建
			m_proxy = renderer->createPrimitiveProxy(this);
		}
	}

}
