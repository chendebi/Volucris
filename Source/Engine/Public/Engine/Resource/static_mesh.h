#ifndef __volucris_static_mesh_h__
#define __volucris_static_mesh_h__

#include <memory>
#include <unordered_map>
#include <Engine/Resource/resource_object.h>
#include <Engine/Resource/soft_object_ptr.h>

namespace volucris
{
	class Material;
	class MeshResource;
	class MeshResourceData;

	class StaticMesh : public ResourceObject
	{
	public:
		StaticMesh();

		StaticMesh(const std::shared_ptr<StaticMesh>& parent);

		StaticMesh(const std::shared_ptr<MeshResource>& resource);

		bool setMaterial(const std::string& slot, const TSoftObjectPtr<Material>& mat);

		const std::unordered_map<std::string, TSoftObjectPtr<Material>>& getMaterials() const { return m_materials; }

		std::shared_ptr<MeshResource> getResource() const;

		bool serialize(Serializer& serializer) const override;

		void deserialize(Serializer& serializer) override;

	private:
		void setMeshResource(const std::shared_ptr<MeshResource>& resource);

	private:
		std::shared_ptr<StaticMesh> m_parent = nullptr;
		std::shared_ptr<MeshResource> m_resource;
		std::unordered_map<std::string, TSoftObjectPtr<Material>> m_materials;
	};
}

#endif // !__volucris_static_mesh_h__
