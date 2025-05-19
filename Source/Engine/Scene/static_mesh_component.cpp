#include "Scene/static_mesh_component.h"
#include "Resource/static_mesh.h"

namespace volucris
{
	StaticMeshComponent::StaticMeshComponent(const std::shared_ptr<StaticMesh>& mesh)
		: PrimitiveComponent()
		, m_mesh(mesh)
	{
		setMeshResource(mesh->getResource());
		setMaterials(mesh->getMaterials());
	}
}

