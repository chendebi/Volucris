#include "Scene/primitive_component.h"
#include "Core/volucris.h"
#include <Core/assert.h>
#include <Application/application.h>
#include <Renderer/renderer.h>
#include "Scene/scene.h"
#include "Engine/Resource/mesh_resource_data.h"
#include "Resource/material.h"
#include "Renderer/scene_proxy.h"

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
		auto sceneProxy = getScene()->getSceneProxy();
		if (m_proxy)
		{
			if (sceneProxy)
			{
				renderer->pushCommand([sceneProxy, proxy = m_proxy]() {
					sceneProxy->removePrimitiveProxy(proxy);
					});
			}

			for (const auto& [slot, mat] : m_meshData->getMaterials())
			{
				mat->deattachProxy();
			}
			m_proxy = nullptr;
		}

		if (!getScene()->getSceneProxy())
		{
			return;
		}

		if (isAttached() && m_meshData)
		{
			// 创建
			m_proxy = renderer->createPrimitiveProxy(this);
		}
	}

}
