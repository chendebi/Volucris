#ifndef __volucris_mesh_resource_data_h__
#define __volucris_mesh_resource_data_h__

#include <glm/glm.hpp>
#include <vector>
#include <Core/types_help.h>
#include <string>
#include <memory>
#include <unordered_map>

namespace volucris
{
	class Material;

	struct Section
	{
		std::vector<uint32> indices;
		std::string slot;
	};

	class MeshResourceData
	{
	public:
		MeshResourceData(size_t count);

	private:
		std::vector<glm::vec3> m_vertices;
		std::vector<Section> m_sections;
		std::unordered_map<std::string, std::shared_ptr<Material>> m_materials;
	};
}

#endif // !__volucris_mesh_resource_data_h__
