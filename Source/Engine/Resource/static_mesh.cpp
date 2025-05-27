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
		: ResourceObject()
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
		if (m_parent)
		{
			serializer << 1 << m_parent->getMetaData().guid;
		}
		else
		{
			serializer << 2;
			if (!m_resource->serialize(serializer))
			{
				return false;
			}
		}

		uint32 matCount = m_materials.size();
		serializer.serialize(matCount);
		for (const auto& [slot, mat] : m_materials)
		{
			ResourceRegistry::Instance().makesureDependenceValid(mat);
			serializer.serialize(slot);
			ResourceRegistry::Instance().serializeDependenceTo(serializer, mat);
		}

		return true;
	}

	void StaticMesh::deserialize(Serializer& serializer)
	{
		int flag = 0;
		serializer.deserialize(flag);
		if (flag == 1)
		{
			std::string guid;
			serializer.deserialize(guid);
			m_parent = ResourceRegistry::Instance().loadResource<StaticMesh>(GUID(guid));
			setMeshResource(m_parent->getResource());
		}
		else if (flag == 2)
		{
			auto resource = std::make_shared<MeshResource>();
			resource->deserialize(serializer);
			setMeshResource(resource);
		}
		else
		{
			V_LOG_WARN(Engine, "deserialize static mesh failed.");
			return;
		}

		uint32 matCount = 0;
		serializer.deserialize(matCount);
		for (auto idx = 0; idx < matCount; ++idx)
		{
			std::string slot, guid;
			serializer.deserialize(slot);
			serializer.deserialize(guid);
			auto material = ResourceRegistry::Instance().loadResource<Material>(GUID(guid));
			setMaterial(slot, material);
		}
	}
}
