#include "Game/StaticMesh.h"
#include <Render/StaticMeshProxy.h>

namespace volucris
{
	StaticMesh::StaticMesh()
		: GameObject()
		, m_proxy()
	{

	}

	void StaticMesh::addPrimitiveInfo(PrimitiveInfo info)
	{
		m_primitiveInfos.emplace_back(std::move(info));
	}

	std::shared_ptr<StaticMeshProxy> StaticMesh::getProxy()
	{
		std::shared_ptr<StaticMeshProxy> proxy = nullptr;
		if (!m_proxy.expired())
		{
			proxy = m_proxy.lock();
		}

		if (!proxy)
		{

		}
		return nullptr;
	}


}

BOOST_CLASS_EXPORT_IMPLEMENT(volucris::StaticMesh)