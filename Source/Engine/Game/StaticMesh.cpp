#include "Game/StaticMesh.h"
#include <Render/StaticMeshProxy.h>
// 必须包含的存档类型
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

#include <Render/Renderer.h>

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
			PrimitiveInfo info = m_data.build();
			buildSubMeshData(info);

			proxy = std::make_shared<StaticMeshProxy>();
			Renderer::getInstance().push([proxy, info]() {
				proxy->init(info);
				});
			m_proxy = proxy;
		}
		return proxy;
	}

	void StaticMesh::buildSubMeshData(PrimitiveInfo& info)
	{
		size_t capacity = 0;
		for (auto& submesh : m_submeshes)
		{
			capacity += submesh->getBufferSize();
		}
		info.segmentData.resize(capacity);
		size_t offset = 0;
		for (auto& submesh : m_submeshes)
		{
			PrimitiveSegment segment;
			segment.mode = submesh->getDrawMode();
			segment.count = submesh->getCount();
			segment.offset = offset;
			segment.type = submesh->getDataType();
			const auto& size = submesh->getBufferSize();
			memcpy(info.segmentData.data()+offset, submesh->getData(), size);
			offset += size;
			info.segments.push_back(segment);
		}
	}
}

BOOST_CLASS_EXPORT_IMPLEMENT(volucris::StaticMesh)