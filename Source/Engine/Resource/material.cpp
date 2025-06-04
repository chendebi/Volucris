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

	Material::~Material()
	{
	}

	void Material::setShaderPath(const std::string& vs, const std::string& fs)
	{
		m_vsFilePath = vs;
		m_fsFilePath = fs;
	}

	std::shared_ptr<MaterialProxy> Material::getRenderProxy()
	{
		auto proxy = m_proxy.lock();
		if (!proxy)
		{
			proxy = std::make_shared<MaterialProxy>(this);
			m_proxy = proxy;

			V_LOG_DEBUG(Engine, "create material: {}", getAsset().getAssetPath())
		}
		return proxy;
	}

	void Material::update()
	{
		if (isDirty())
		{
			auto proxy = getRenderProxy();

			gApp->getRenderer()->pushCommand([proxy, data = m_parameterData]() {
				proxy->updateParameters(data);
				});
			m_dirty = false;
		}
	}

	MaterialParameter* Material::getParameterByName(const std::string& name)
	{
		for (const auto& parameter : m_parameters)
		{
			if (parameter->getDescription().name == name)
			{
				return parameter.get();
			}
		}
		return nullptr;
	}

	MaterialParameter* Material::getParameterByType(MaterialParameterDesc::Type type)
	{
		for (const auto& parameter : m_parameters)
		{
			if (parameter->getDescription().type == type)
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
			if (m_parent->getMetaData().guid.empty())
			{
				return false;
			}
			serializer << 1 << m_parent->getMetaData().guid;
		}
		else
		{
			serializer << 2 << m_vsFilePath << m_fsFilePath;
			m_resource->serialize(serializer);
		}
		serializer.serialize(m_parameterData.data(), m_parameterData.size());
		return true;
	}

	void Material::deserialize(Serializer& serializer)
	{
		int type = 0;
		if (!serializer.deserialize(type))
		{
			return;
		}

		if (type == 1)
		{
			// Load material from GUID
			std::string guid;
			if (!serializer.deserialize(guid))
			{
				return;
			}

			if (auto parent = ResourceRegistry::Instance().loadResource<Material>(GUID(guid)))
			{
				m_parent = parent;
				size_t paramDataSize;
				serializer.deserialize(m_parameterData, paramDataSize);
				setMaterialResource(parent->getResource());
			}
		}
		else if (type == 2)
		{
			// Load material from shader paths
			if (serializer.deserialize(m_vsFilePath) && serializer.deserialize(m_fsFilePath))
			{
				auto resource = std::make_shared<MaterialResource>();
				resource->deserialize(serializer);
				size_t paramDataSize;
				serializer.deserialize(m_parameterData, paramDataSize);
				setMaterialResource(resource);
			}
			else
			{
				V_LOG_ERROR(Engine, "Failed to deserialize shader paths for material");
				return;
			}
		}
		
	}

	void Material::setMaterialResource(const std::shared_ptr<MaterialResource>& resource)
	{
		//check(!m_resource)
		m_resource = resource;
		size_t tableSize = 0;
		std::vector<MaterialParameterDesc> descriptions;
		std::vector<MaterialParameterDesc> textureDescriptions;
		for (auto desc : resource->getParameterDescriptions())
		{
			auto size = MaterialParameterDesc::sizeOfType(desc.type);
			if (desc.isTextureValue())
			{
				textureDescriptions.push_back(desc);
			}
			else
			{
				if (size > 0)
				{
					desc.offset = tableSize;
					tableSize += size;
				}
				descriptions.push_back(desc);
			}
		}

		m_parameterData.resize(tableSize);
		m_parameters.clear();
		m_textureParameters.clear();
		for (const auto& desc : descriptions)
		{
			m_parameters.push_back(std::make_unique<MaterialValueParameter>(this, desc, m_parameterData.data()));
		}

		for (const auto& desc : textureDescriptions)
		{
			// TODO: 替换为默认贴图
			m_textureParameters.push_back(std::make_unique<MaterialTextureParameter>(this, desc, ""));
		}
	}
}
