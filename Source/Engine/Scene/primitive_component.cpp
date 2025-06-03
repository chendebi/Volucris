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

namespace volucris
{
	PrimitiveComponent::PrimitiveComponent()
		: SceneComponent()
		, m_proxy(nullptr)
		, m_resource(nullptr)
	{
	}

	void PrimitiveComponent::setMeshResource(const std::shared_ptr<MeshResource>& resource)
	{
		if (m_proxy)
		{
			V_LOG_WARN(Engine, "not support set mesh resource while rendering");
			return;
		}

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

			m_proxy = nullptr;
		}

		if (!getScene()->getSceneProxy())
		{
			return;
		}

		if (isAttached() && m_resource)
		{
			for (const auto& [slot, mat] : m_materials)
			{
				const auto& param = mat->getParameterByType(MaterialParameterDesc::MODEL_INFO);
				if (!param)
				{
					V_LOG_WARN(Engine, "material with no model uniform for primitive component.");
				}
				else
				{
					m_modelMatParameters.push_back(dynamic_cast<MaterialValueParameter*>(param));
				}
			}
			onTransformChanged();
			// 创建
			m_proxy = renderer->createPrimitiveProxy(this);
		}
	}

	void PrimitiveComponent::onTransformChanged()
	{
		const auto& modelMat = getWorldTransform();
		for (const auto& parameter : m_modelMatParameters)
		{
			parameter->setValue(modelMat);
		}
	}

	void PrimitiveComponent::updateTransform()
	{
		for (const auto& parameter : m_modelMatParameters)
		{
			parameter->getMaterial()->updateParametersToRenderer();
		}
	}

}
