#include "Renderer/material_proxy.h"
#include "Resource/material.h"

namespace volucris
{

	MaterialResourceProxy::MaterialResourceProxy(MaterialResource* resource)
		: std::enable_shared_from_this<MaterialResourceProxy>()
		, ProxyObject()
		, m_vss()
		, m_fss()
	{
		resource->setProxy(this);
		setSceneObject(resource);
		m_vss = resource->getVertexShaderSource();
		m_fss = resource->getFragmentShaderSource();
	}

	MaterialProxy::MaterialProxy(Material* material)
		: ProxyObject()
		, m_resource(nullptr)
	{
		setSceneObject(material);
		material->setProxy(this);

		auto resource = material->getResource();
		if (auto proxy = (MaterialResourceProxy*)resource->getProxy())
		{
			m_resource = proxy->getShared();
		}
		else
		{
			m_resource = std::make_shared<MaterialResourceProxy>(material->getResource().get());
		}
	}
}
