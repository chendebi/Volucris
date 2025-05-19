#ifndef __volucris_mesh_proxy_h__
#define __volucris_mesh_proxy_h__

#include <memory>
#include "Engine/Renderer/mesh_render_data.h"

namespace volucris
{
	class MeshResourceData;
	class OGLBufferObject;
	class OGLVertexArrayObject;

	struct MeshRenderInfo
	{
		std::shared_ptr<OGLBufferObject> vbo;
		std::shared_ptr<OGLBufferObject> ebo;
		std::shared_ptr<OGLVertexArrayObject> vao;

		MeshRenderInfo();
	};

	class MeshProxy
	{
	public:
		MeshProxy(MeshResourceData* resource);

		MeshRenderInfo* getRenderInfo() const { return m_renderInfo.get(); }

	private:
		std::shared_ptr<MeshRenderData> m_renderData;
		std::unique_ptr<MeshRenderInfo> m_renderInfo;
	};
}

#endif // !__volucris_mesh_proxy_h__
