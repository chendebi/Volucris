#ifndef __volucris_primitive_proxy_h__
#define __volucris_primitive_proxy_h__

#include <memory>

namespace volucris
{
	class PrimitiveComponent;
	class MeshRenderData;

	class PrimitiveProxy
	{
	public:
		PrimitiveProxy(PrimitiveComponent* Primitive);

	private:
		std::shared_ptr<MeshRenderData> m_renderData;
	};
}

#endif // !__volucris_primitive_proxy_h__
