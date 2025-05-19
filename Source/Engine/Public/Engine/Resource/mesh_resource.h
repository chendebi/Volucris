#ifndef __volucris_mesh_resource_h__
#define __volucris_mesh_resource_h__

#include <memory>
#include "Engine/Resource/render_resource.h"

namespace volucris
{
	class MeshProxy;
	class MeshResourceData;

	class MeshResource : public RenderResource
	{
	public:
		MeshResource(const std::shared_ptr<MeshResourceData>& data);

		MeshProxy* getProxy() const { return m_proxy; }

		MeshProxy* attachProxy();

		void deattachProxy();

		std::shared_ptr<MeshResourceData> getResourceData() const { return m_data; }

	protected:
		void releaseRenderProxy() override;

	private:
		std::shared_ptr<MeshResourceData> m_data;
		MeshProxy* m_proxy;
	};
}

#endif // !__volucris_mesh_resource_h__
