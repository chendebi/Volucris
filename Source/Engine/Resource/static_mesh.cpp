#include "Resource/static_mesh.h"
#include "Core/volucris.h"
#include "Core/assert.h"
#include "Renderer/mesh_proxy.h"
#include <Application/application.h>
#include <Renderer/renderer.h>
#include "Resource/mesh_resource_data.h"
#include "Resource/mesh_resource.h"
#include <Resource/material.h>
#include <Resource/resource_registry.h>

namespace volucris
{
	StaticMesh::StaticMesh()
		: ResourceObject(AssetType::STATIC_MESH)
		, m_resource(nullptr)
	{

	}

	void StaticMesh::setMeshResource(const std::shared_ptr<MeshResource>& resource)
	{
		checkf(m_resource == nullptr, Engine, "static mesh try set mesh resource while it has been set");
		m_materials.clear();
		m_resource = resource;
		for (const auto& section : m_resource->getResourceData()->getSections())
		{
			// TODO: 设置为默认材质
			m_materials[section.slot] = nullptr;
		}
	}

	bool StaticMesh::setMaterial(const std::string& slot, const std::shared_ptr<Material>& mat)
	{
		auto it = m_materials.find(slot);
		if (it == m_materials.end())
		{
			V_LOG_WARN(Engine, "static mesh set material failed. not found slot: {}", slot);
			return false;
		}
		it->second = mat;
		return true;
	}

	std::shared_ptr<MeshResource> StaticMesh::getResource() const
	{
		return m_resource;
	}

	bool StaticMesh::serialize(Serializer& serializer) const
	{
		

		return true;
	}

	void StaticMesh::deserialize(Serializer& serializer)
	{
		
	}
}
