#ifndef __volucris_static_mesh_h__
#define __volucris_static_mesh_h__

#include <Engine/Game/GameObject.h>
#include <Engine/Core/PrimitiveInfo.h>
#include <Engine/Game/MeshResourceData.h>

namespace volucris
{
	class StaticMeshProxy;

	class StaticMesh : public GameObject
	{
	public:
		StaticMesh();

		void setMeshData(MeshData data);

		template <class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& m_data;
		}

		std::shared_ptr<StaticMeshProxy> getProxy();

	private:
		MeshData m_data;
		std::weak_ptr<StaticMeshProxy> m_proxy;
	};
}

#endif // !__volucris_static_mesh_h__
