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
		: ResourceObject(Asset::STATIC_MESH)
		, m_resource(nullptr)
	{

	}

	StaticMesh::StaticMesh(const std::shared_ptr<StaticMesh>& parent)
		: StaticMesh()
	{
		m_parent = parent;
		setMeshResource(parent->getResource());
	}

	StaticMesh::StaticMesh(const std::shared_ptr<MeshResource>& resource)
		: StaticMesh()
	{
		setMeshResource(resource);
	}

	void StaticMesh::setMeshResource(const std::shared_ptr<MeshResource>& resource)
	{
		checkf(m_resource == nullptr, Engine, "static mesh try set mesh resource while it has been set");
		m_materials.clear();
		m_resource = resource;
		for (const auto& section : m_resource->getResourceData()->getSections())
		{
			// TODO: 设置为默认材质
			m_materials[section.slot] = TSoftObjectPtr<Material>("");
		}
	}

	bool StaticMesh::setMaterial(const std::string& slot, const TSoftObjectPtr<Material>& mat)
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
		if (m_parent)
		{
			serializer << (int32)1 << m_parent->getAsset().getAssetPath();
		}
		else if (m_resource)
		{
			serializer << (int32)2;
			m_resource->serialize(serializer);
		}
		else
		{
			return false;
		}

		serializer.serialize((int32)m_materials.size());

		for (const auto& [slot, material] : m_materials)
		{
			serializer.serialize(slot);
			serializer.serialize(material);
		}
		
		return true;
	}

	void StaticMesh::deserialize(Serializer& serializer)
	{
		int32 type = 0;
		serializer >> type;
		if (type == 1)
		{
			std::string parentPath;
			serializer >> parentPath;
			if (auto parent = ResourceRegistry::Instance().loadResource<StaticMesh>(parentPath))
			{
				m_parent = parent;
				setMeshResource(parent->getResource());
			}
		}
		else if (type == 2)
		{
			auto resource = std::make_shared<MeshResource>();
			resource->deserialize(serializer);
			setMeshResource(resource);
		}
		else 
		{
			V_LOG_ERROR(Engine, "deserialize static mesh failed. type: {}", type);
			return;
		}

		int32 materialCount = 0;
		serializer.deserialize(materialCount);

		for (auto idx = 0; idx < materialCount; ++idx)
		{
			std::string slot;
			serializer.deserialize(slot);
			TSoftObjectPtr<Material> material;
			material.deserialize(serializer);
			setMaterial(slot, material);
		}
	}
}
