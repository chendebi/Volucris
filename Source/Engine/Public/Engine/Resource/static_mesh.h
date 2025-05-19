#ifndef __volucris_static_mesh_h__
#define __volucris_static_mesh_h__

#include <memory>
#include <unordered_map>
#include "Engine/Resource/render_resource.h"

namespace volucris
{
	class Material;
	class MeshResource;
	class MeshResourceData;

	class StaticMesh : public ResourceObject
	{
	public:
		StaticMesh();

		void setMeshResource(const std::shared_ptr<MeshResource>& resource);

		bool setMaterial(const std::string& slot, const std::shared_ptr<Material>& mat);

		const std::unordered_map<std::string, std::shared_ptr<Material>>& getMaterials() const { return m_materials; }

		std::shared_ptr<MeshResource> getResource() const;

	private:
		std::shared_ptr<MeshResource> m_resource;
		std::unordered_map<std::string, std::shared_ptr<Material>> m_materials;
	};
}

#endif // !__volucris_static_mesh_h__
