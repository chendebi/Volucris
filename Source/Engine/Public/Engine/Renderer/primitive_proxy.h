#ifndef __volucris_primitive_proxy_h__
#define __volucris_primitive_proxy_h__

#include <memory>

namespace volucris
{
	class PrimitiveComponent;
	class MeshRenderData;

	class OGLBufferObject;
	class OGLVertexArrayObject;

	struct PrimitiveRenderInfo
	{
		std::shared_ptr<OGLBufferObject> vbo;
		std::shared_ptr<OGLBufferObject> ebo;
		std::shared_ptr<OGLVertexArrayObject> vao;

		PrimitiveRenderInfo();
	};

	struct SectionDrawData
	{
		PrimitiveRenderInfo* renderInfo;
		SectionRenderData* section;
	};

	class PrimitiveProxy
	{
	public:
		PrimitiveProxy(PrimitiveComponent* Primitive);

		PrimitiveRenderInfo getRenderInfo() const { return m_renderInfo; }

	private:
		std::shared_ptr<MeshRenderData> m_renderData;
		PrimitiveRenderInfo m_renderInfo;
	};
}

#endif // !__volucris_primitive_proxy_h__
