#ifndef __volucris_mesh_resource_h__
#define __volucris_mesh_resource_h__

#include <memory>
#include <Engine/Resource/resource_object.h>

namespace volucris
{
	class MeshProxy;
	class MeshResourceData;

	class MeshResource : public ResourceObject
	{
	public:
		MeshResource(const std::shared_ptr<MeshResourceData>& data = nullptr);

		std::shared_ptr<MeshProxy> getRenderProxy();

		std::shared_ptr<MeshResourceData> getResourceData() const { return m_data; }

		bool serialize(Serializer& serializer) const override;

		void deserialize(Serializer& serializer) override;

	private:
		std::shared_ptr<MeshResourceData> m_data;
		std::weak_ptr<MeshProxy> m_proxy;
	};
}

#endif // !__volucris_mesh_resource_h__
