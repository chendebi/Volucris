#include "Resource/material.h"
#include "Resource/material_resource.h"
#include "Core/volucris.h"
#include "Renderer/material_proxy.h"
#include <Application/application.h>
#include "Renderer/renderer.h"
#include "Core/assert.h"

namespace volucris
{
	Material::Material()
		: m_resource(nullptr)
		, m_parameters()
		, m_proxy(nullptr)
	{
		
	}

	Material::~Material()
	{
	}

	MaterialProxy* Material::attachProxy()
	{
		if (!m_proxy)
		{
			m_proxy = new MaterialProxy(this);
			m_proxy->setResource(m_resource->attachProxy());
		}
		addRenderRef();
		V_LOG_DEBUG(Engine, "attach to material: {}, {}", getResourceFullPath(), getRenderRefCount())
		return m_proxy;
	}

	void Material::deattachProxy()
	{
		if (m_proxy)
		{
			removeRenderRef();
		}
	}

	void Material::updateParametersToRenderer()
	{
		if (m_proxy)
		{
			gApp->getRenderer()->pushCommand([proxy = m_proxy, data = m_parameterData]() {
				proxy->updateParameters(data);
				});
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

	void Material::releaseRenderProxy()
	{
		if (m_proxy)
		{
			gApp->getRenderer()->pushCommand([proxy = m_proxy]() {
				delete proxy;
				});
			m_resource->deattachProxy();
			V_LOG_DEBUG(Engine, "release material: {}", getResourceFullPath())
			m_proxy = nullptr;
		}
	}

	bool Material::serialize(rapidjson::Value& serializer, rapidjson::Document::AllocatorType& allocator) const
	{
		if (m_parent)
		{
			serializeDependence(serializer, allocator, "parent", m_parent);
		}
		else
		{
			rapidjson::Value source(rapidjson::kObjectType);
			source.AddMember(rapidjson::StringRef("vs"), rapidjson::StringRef(m_vsFilePath.c_str()), allocator);
			source.AddMember(rapidjson::StringRef("fs"), rapidjson::StringRef(m_fsFilePath.c_str()), allocator);
			serializer.AddMember(rapidjson::StringRef("source"), source, allocator);
		}
		return true;
	}

	void Material::deserialize(const rapidjson::Value& serializer, rapidjson::Document::AllocatorType& allocator)
	{
		if (auto parent = loadDependence(serializer, "parent"))
		{
			m_parent = std::dynamic_pointer_cast<Material>(parent);
			m_resource = m_parent->getResource();
		}
		else
		{
			rapidjson::Value source;
			if (!getJsonValueObject(serializer, allocator, "source", source))
			{
				return;
			}

			if (!getJsonValueString(source, "vs", m_vsFilePath)
				|| !getJsonValueString(source, "fs", m_fsFilePath))
			{
				return;
			}

			MaterialResourceLoader loader;
			if (auto resource = loader.loadMaterialResource(m_vsFilePath, m_fsFilePath))
			{
				setMaterialResource(resource);
			}
		}
	}
	void Material::setMaterialResource(const std::shared_ptr<MaterialResource>& resource)
	{
		check(!m_resource)
		m_resource = resource;
		size_t tableSize = 0;
		std::vector<MaterialParameterDesc> descriptions;
		for (auto desc : resource->getParameterDescriptions())
		{
			auto size = MaterialParameterDesc::sizeOfType(desc.type);
			if (size > 0)
			{
				desc.offset = tableSize;
				tableSize += size;
			}
			descriptions.push_back(desc);
		}

		m_parameterData.resize(tableSize);
		for (const auto& desc : descriptions)
		{
			m_parameters.push_back(std::make_unique<MaterialParameter>(this, desc, m_parameterData.data()));
		}
	}
}
