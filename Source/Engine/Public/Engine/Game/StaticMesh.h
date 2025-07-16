#ifndef __volucris_static_mesh_h__
#define __volucris_static_mesh_h__

#include <Engine/Game/GameObject.h>
#include <Engine/Core/PrimitiveInfo.h>

namespace volucris
{
	class StaticMeshProxy;

	class StaticMesh : public GameObject
	{
	public:
		StaticMesh();

		void addPrimitiveInfo(PrimitiveInfo info);

		template <class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& m_primitiveInfos;
		}

		std::shared_ptr<StaticMeshProxy> getProxy();

	private:
		std::vector<PrimitiveInfo> m_primitiveInfos;
		std::weak_ptr<StaticMeshProxy> m_proxy;
	};
}

#endif // !__volucris_static_mesh_h__
