#include "Resource/material.h"
#include "Core/volucris.h"
#include "Renderer/material_proxy.h"
#include <Application/application.h>
#include "Renderer/renderer.h"
#include "Core/assert.h"

namespace volucris
{
	MaterialResource::MaterialResource()
		: RenderResource()
		, m_vss()
		, m_fss()
		, m_proxy(nullptr)
	{
	}

	MaterialResource::~MaterialResource()
	{
	}

	MaterialResource::MaterialResource(const std::string& vss, const std::string& fss)
		: MaterialResource()
	{
		setSource(vss, fss);
	}

	void MaterialResource::setSource(const std::string& vss, const std::string& fss)
	{
		m_vss = vss;
		m_fss = fss;
	}

	MaterialResourceProxy* MaterialResource::attachProxy()
	{
		if (!m_proxy)
		{
			m_proxy = new MaterialResourceProxy(this);
		}
		addRenderRef();
		V_LOG_DEBUG(Engine, "attach to material resource: {}, {}", getResourceFullPath(), getRenderRefCount())
		return m_proxy;
	}

	void MaterialResource::deattachProxy()
	{
		if (m_proxy)
		{
			removeRenderRef();
		}
	}

	void MaterialResource::releaseRenderProxy()
	{
		if (m_proxy)
		{
			gApp->getRenderer()->pushCommand([proxy = m_proxy]() {
				delete proxy;
				});
			V_LOG_DEBUG(Engine, "release material resource proxy: {}", getResourceFullPath())
		}
		m_proxy = nullptr;
	}

	Material::Material(const std::shared_ptr<MaterialResource>& resource)
		: m_resource(resource)
		, m_parameters()
		, m_proxy(nullptr)
	{
		size_t tableSize = 0;
		std::vector<MaterialParameterDesc> descriptions;
		for(auto desc : resource->getParameterDescriptions())
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
}
