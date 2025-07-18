#include "Game/StaticMesh.h"
#include <Render/StaticMeshProxy.h>
// 必须包含的存档类型
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
namespace volucris
{
	StaticMesh::StaticMesh()
		: GameObject()
		, m_proxy()
	{

	}

	void StaticMesh::setMeshData(MeshData data)
	{
		m_data = std::move(data);
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