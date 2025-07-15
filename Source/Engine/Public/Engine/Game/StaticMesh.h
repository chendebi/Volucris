#ifndef __volucris_static_mesh_h__
#define __volucris_static_mesh_h__

#include <Engine/Game/GameObject.h>
#include <Engine/Core/PrimitiveInfo.h>

namespace volucris
{
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

	private:
		std::vector<PrimitiveInfo> m_primitiveInfos;
	};
}

#endif // !__volucris_static_mesh_h__
