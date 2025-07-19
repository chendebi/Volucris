#ifndef __volucris_static_mesh_component_h__
#define __volucris_static_mesh_component_h__

#include <Engine/Game/SceneComponent.h>

namespace volucris
{
	class StaticMesh;

	class StaticMeshComponent : public SceneComponent
	{
	public:
		StaticMeshComponent();

	private:
		std::shared_ptr<StaticMesh> m_mesh;
	};
}

#endif // !__volucris_static_mesh_component_h__
