#include "Scene/static_mesh_component.h"
#include "Resource/static_mesh.h"
#include <Resource/material.h>

namespace volucris
{
	StaticMeshComponent::StaticMeshComponent(const std::shared_ptr<StaticMesh>& mesh)
		: PrimitiveComponent()
		, m_mesh(mesh)
	{
		setMeshResource(mesh->getResource());
		for (auto [slot, mat] : mesh->getMaterials())
		{
			setMaterial(slot, mat.tryLoad());
		}
		//setMaterials(mesh->getMaterials());
	}
}

