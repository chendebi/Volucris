#include "Resource/mesh_resource.h"
#include "Renderer/mesh_proxy.h"
#include "Application/application.h"
#include "Renderer/renderer.h"
#include "Core/volucris.h"

namespace volucris
{
	MeshResource::MeshResource(const std::shared_ptr<MeshResourceData>& data)
		: ResourceObject()
		, m_data(data)
		, m_proxy()
	{
	}

	std::shared_ptr<MeshProxy> MeshResource::getRenderProxy()
	{
		std::shared_ptr<MeshProxy> proxy = m_proxy.lock();
		if (!proxy)
		{
			proxy = std::make_shared<MeshProxy>();
			m_proxy = proxy;
			V_LOG_DEBUG(Engine, "create mesh resource proxy: {}", getResourceFullPath());
		}
		return proxy;
	}
}