#include "Game/StaticMesh.h"

namespace volucris
{
	StaticMesh::StaticMesh()
		: GameObject()
	{

	}

	void StaticMesh::addPrimitiveInfo(PrimitiveInfo info)
	{
		m_primitiveInfos.emplace_back(std::move(info));
	}


}

BOOST_CLASS_EXPORT_IMPLEMENT(volucris::StaticMesh)