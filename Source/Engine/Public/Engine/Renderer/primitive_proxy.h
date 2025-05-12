#ifndef __volucris_primitive_proxy_h__
#define __volucris_primitive_proxy_h__

#include <memory>
#include "Engine/Renderer/proxy_object.h"

namespace volucris
{
	class PrimitiveComponent;
	class MeshRenderData;

	class PrimitiveProxy : public ProxyObject
	{
	public:
		PrimitiveProxy(PrimitiveComponent* Primitive);

	private:
		std::shared_ptr<MeshRenderData> m_renderData;
	};
}

#endif // !__volucris_primitive_proxy_h__
