#include "Resource/material.h"
#include "Resource/material_resource.h"
#include "Core/volucris.h"
#include "Renderer/material_proxy.h"
#include <Application/application.h>
#include "Renderer/renderer.h"
#include "Core/assert.h"
#include <Resource/resource_registry.h>

namespace volucris
{
	Material::Material()
		: ResourceObject(AssetType::MATERIAL)
		, m_resource(nullptr)
		, m_parameters()
		, m_proxy()
	{
		
	}

	Material::Material(const std::shared_ptr<MaterialResource>& resource)
		: Material()
	{
		setMaterialResource(resource);
	}

	Material::Material(const std::shared_ptr<Material>& parent)
		: Material()
	{
		m_parent = parent;
		setMaterialResource(m_parent->getResource());
	}

	Material::~Material()
	{
#if WITH_EDITOR
		if (m_resource)
		{
			m_resource->Rebuild.removeAll(this);
		}
#endif
	}

	std::shared_ptr<MaterialProxy> Material::getRenderProxy()
	{
		auto proxy = m_proxy.lock();
		if (!proxy)
		{
			proxy = std::make_shared<MaterialProxy>();
			MaterialParameterRenderData renderData;
			for (const auto& parameter : m_parameters)
			{
				renderData.values.push_back(parameter->createUniformValue());
			}
			renderData.textures = {};
			proxy->updateParameterRenderData(renderData);
			m_proxy = proxy;

			V_LOG_DEBUG(Engine, "create material: {}", getAsset().getAssetPath())
		}
		return proxy;
	}

	void Material::update()
	{
		if (isDirty())
		{
			if (auto proxy = m_proxy.lock())
			{
				MaterialParameterRenderData renderData;
				for (const auto& parameter : m_parameters)
				{
					renderData.values.push_back(parameter->createUniformValue());
				}
				renderData.textures = {};
				gApp->getRenderer()->pushCommand([proxy, renderData]() {
					proxy->updateParameterRenderData(std::move(renderData));
					});
			}
			markDirty(false);
		}
		else
		{
			bool update = false;
			for (const auto& paramter : m_parameters)
			{
				if (paramter->isDirty())
				{
					update = true;
				}
			}
			if (!update)
			{
				return;
			}
			if (auto proxy = m_proxy.lock())
			{
				MaterialParameterRenderData renderData;
				for (const auto& parameter : m_parameters)
				{
					renderData.values.push_back(parameter->createUniformValue());
				}
				renderData.textures = {};
				gApp->getRenderer()->pushCommand([proxy, renderData]() {
					proxy->updateParameterRenderData(std::move(renderData));
					});
			}
		}
	}

	MaterialParameter* Material::getParameterByName(const std::string& name)
	{
		for (auto& parameter : m_parameters)
		{
			if (parameter->getName() == name)
			{
				return parameter.get();
			}
		}
		return nullptr;
	}

	bool Material::serialize(Serializer& serializer) const
	{
		if (m_parent)
		{
			serializer << 1 << m_parent->getAsset().uuid;
		}
		else
		{
			m_resource->serialize(serializer);
		}
		//serializer.serialize(m_parameterData.data(), m_parameterData.size());
		return true;
	}

	void Material::deserialize(Serializer& serializer)
	{
	}

	void Material::setMaterialResource(const std::shared_ptr<MaterialResource>& resource)
	{
		m_resource = resource;
		onSourceRebuild(resource.get());
#if WITH_EDITOR
		resource->Rebuild.addObject(this, &Material::onSourceRebuild);
#endif
	}

	void Material::onSourceRebuild(MaterialResource* resource)
	{
		check(resource == m_resource.get());
		m_parameters.clear();
		const auto& descriptions = resource->getParameterDescriptions();
		m_parameters.reserve(descriptions.size());
		for (const auto& description : descriptions)
		{
			switch (description.type)
			{
			case MaterialParameterType::FLOAT:
				m_parameters.push_back(std::make_shared<MaterialParameterFloat>(description.name));
				break;
			case MaterialParameterType::VEC2:
				break;
			case MaterialParameterType::VEC3:
				m_parameters.push_back(std::make_shared<MaterialParameterVec3>(description.name));
				break;
			case MaterialParameterType::VEC4:
				break;
			case MaterialParameterType::TEXTURE2D:
				break;
			default:
				break;
			}
		}
		dirty();
	}
}
