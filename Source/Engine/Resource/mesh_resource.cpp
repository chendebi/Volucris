#include "Resource/mesh_resource.h"
#include "Renderer/mesh_proxy.h"
#include "Application/application.h"
#include "Renderer/renderer.h"
#include "Core/volucris.h"

namespace volucris
{
	MeshResource::MeshResource(const std::shared_ptr<MeshResourceData>& data)
		: RenderResource()
		, m_data(data)
		, m_proxy(nullptr)
	{
	}

	MeshProxy* MeshResource::attachProxy()
	{
		if (!m_proxy)
		{
			m_proxy = new MeshProxy(m_data.get());
		}
		addRenderRef();
		return m_proxy;
	}

	void MeshResource::deattachProxy()
	{
		if (m_proxy)
		{
			removeRenderRef();
		}
	}

	void MeshResource::releaseRenderProxy()
	{
		if (m_proxy)
		{
			gApp->getRenderer()->pushCommand([proxy = m_proxy]() {
				delete proxy;
				});
			V_LOG_DEBUG(Engine, "release mesh resource proxy: {}", getResourceFullPath());
			m_proxy = nullptr;
		}
	}
}