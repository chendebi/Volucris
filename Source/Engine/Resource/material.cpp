#include "Resource/material.h"
#include "Core/volucris.h"
#include "Renderer/material_proxy.h"
#include <Application/application.h>
#include "Renderer/renderer.h"

namespace volucris
{
	MaterialResource::MaterialResource()
		: RenderResource()
		, m_vss()
		, m_fss()
		, m_proxy(nullptr)
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
		addRenderRef();
		if (!m_proxy)
		{
			m_proxy = new MaterialResourceProxy(this);
		}
		return m_proxy;
	}

	void MaterialResource::deattachProxy()
	{
		removeRenderRef();
	}

	void MaterialResource::releaseRenderProxy()
	{
		if (m_proxy)
		{
			gApp->getRenderer()->pushCommand([proxy = m_proxy]() {
				delete proxy;
				});
			removeRenderRef();
		}
		m_proxy = nullptr;
	}

	Material::Material(const std::shared_ptr<MaterialResource>& resource)
		: m_resource(resource)
		, m_parameters()
		, m_proxy(nullptr)
	{
		for(const auto& [type, names] : resource->m_parameters)
		{
			for (const auto& name : names)
			{
				if (auto parameter = MaterialParameter::create(type, name))
				{
					m_parameters[name] = std::move(parameter);
				}
				else
				{
					V_LOG_WARN(Engine, "find invalid parameter[{}], type: {}", name, (int)type);
				}
			}
		}
	}

	Material::~Material()
	{
		m_resource->deattachProxy();
	}

	MaterialProxy* Material::attachProxy()
	{
		if (!m_proxy)
		{
			m_proxy = new MaterialProxy(this);
			m_proxy->setResource(m_resource->attachProxy());
		}
		addRenderRef();
		return m_proxy;
	}

	void Material::deattachProxy()
	{
		removeRenderRef();
	}

	void Material::releaseRenderProxy()
	{
		if (m_proxy)
		{
			gApp->getRenderer()->pushCommand([proxy = m_proxy]() {
				delete proxy;
				});
			m_resource->deattachProxy();
			m_proxy = nullptr;
		}
	}
}
