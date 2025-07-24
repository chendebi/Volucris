#include "Render/PrimitiveSceneProxy.h"

namespace volucris
{
	PrimitiveSceneProxy::PrimitiveSceneProxy(const std::shared_ptr<StaticMeshProxy>& mesh)
		: m_meshProxy(mesh)
	{
	}

	PrimitiveSceneProxy::~PrimitiveSceneProxy()
	{
	}
}
