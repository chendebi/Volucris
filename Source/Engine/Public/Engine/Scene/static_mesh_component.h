#ifndef __volucris_static_mesh_component_h__
#define __volucris_static_mesh_component_h__

#include "Engine/Scene/primitive_component.h"

namespace volucris
{
	class StaticMesh;

	class StaticMeshComponent : public PrimitiveComponent
	{
	public:
		StaticMeshComponent(const std::shared_ptr<StaticMesh>& mesh);

	private:
		std::shared_ptr<StaticMesh> m_mesh;
	};
}

#endif // !__volucris_static_mesh_component_h__
