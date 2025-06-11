#include "Scene/primitive_component.h"
#include "Core/volucris.h"
#include <Core/assert.h>
#include <Application/application.h>
#include <Renderer/renderer.h>
#include "Scene/scene.h"
#include "Engine/Resource/mesh_resource_data.h"
#include "Resource/material.h"
#include "Renderer/scene_proxy.h"
#include "Resource/material_parameter.h"
#include "Renderer/material_proxy.h"
#include "Resource/mesh_resource.h"
#include "Resource/material.h"
#include <Renderer/primitive_proxy.h>

namespace volucris
{
	PrimitiveComponent::PrimitiveComponent()
		: SceneComponent()
		, m_proxy()
		, m_resource(nullptr)
	{
	}

	void PrimitiveComponent::setMeshResource(const std::shared_ptr<MeshResource>& resource)
	{
		m_resource = resource;
		std::unordered_map<std::string, std::shared_ptr<Material>> materials;

		for (const auto& section : m_resource->getResourceData()->getSections())
		{
			materials[section.slot] = m_materials[section.slot];
		}
		m_materials = materials;
		markRenderStateDirty();
	}

	bool PrimitiveComponent::setMaterial(const std::string& slot, const std::shared_ptr<Material>& mat)
	{
		m_materials[slot] = mat;
		return true;
	}

	void PrimitiveComponent::setMaterials(const std::unordered_map<std::string, std::shared_ptr<Material>>& mats)
	{
		m_materials = mats;
	}

	Material* PrimitiveComponent::getMaterial(const std::string& slot) const
	{
		auto it = m_materials.find(slot);
		if (it != m_materials.end())
		{
			return it->second.get();
		}
		return nullptr;
	}

	void PrimitiveComponent::updateRenderState()
	{
		auto proxy = m_proxy.lock();
		if (!proxy)
		{
			proxy = std::make_shared<PrimitiveProxy>();
			proxy->initialize(this);
			PrimitiveInfo info;
			info.localToWorldMatrix = getWorldTransform();
			proxy->setPrimitiveInfo(info);
			m_proxy = proxy;


			auto renderer = gApp->getRenderer();
			auto sceneProxy = getScene()->getSceneProxy();
			
			renderer->pushCommand([sceneProxy, proxy]() {
				sceneProxy->addPrimitiveProxy(proxy);
				});

			return;
		}

	}

	void PrimitiveComponent::onTransformChanged()
	{
		const auto& modelMat = getWorldTransform();
		
	}

	void PrimitiveComponent::updateTransform()
	{
		if (auto proxy = m_proxy.lock())
		{
			PrimitiveInfo info;
			info.localToWorldMatrix = getWorldTransform();
			gApp->getRenderer()->pushCommand([proxy, info]() {
				proxy->setPrimitiveInfo(info);
				});
		}
	}
}
