#include "Renderer/material_proxy.h"
#include "Resource/material.h"

namespace volucris
{

	MaterialResourceProxy::MaterialResourceProxy(MaterialResource* resource)
		: m_vss(resource->getVertexShaderSource())
		, m_fss(resource->getFragmentShaderSource())
	{

	}

	MaterialProxy::MaterialProxy(Material* material)
		: m_resource(nullptr)
	{
		
	}

	MaterialProxy::~MaterialProxy()
	{

	}
}
