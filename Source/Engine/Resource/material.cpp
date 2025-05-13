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
