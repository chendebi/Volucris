#ifndef __volucris_mesh_resource_data_h__
#define __volucris_mesh_resource_data_h__

#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <memory>
#include <unordered_map>
#include <Engine/Core/types_help.h>
#include "Engine/Renderer/mesh_render_data.h"

namespace volucris
{
	class Material;

	struct Section
	{
		DrawMode mode;
		std::vector<uint32> indices;
		std::string slot;

		Section() : mode(DrawMode::TRIANGLES), indices(), slot() {}

		Section(void* data, size_t size);
	};

	class MeshResourceData
	{
	public:
		MeshResourceData();

		void setVertices(void* vertices, size_t size);

		void addSection(const std::shared_ptr<Section>& section)
		{
			m_sections.push_back(section);
		}

		std::shared_ptr<MeshRenderData> build();

		const std::vector<std::shared_ptr<Section>>& getSections() const { return m_sections; }

		std::shared_ptr<Material> getMaterial(const std::string& slot) const
		{
			auto it = m_materials.find(slot);
			if (it == m_materials.end())
			{
				return nullptr;
			}
			return it->second;
		}

	private:
		std::vector<glm::vec3> m_vertices;
		std::vector<std::shared_ptr<Section>> m_sections;
		std::unordered_map<std::string, std::shared_ptr<Material>> m_materials;
	};
}

#endif // !__volucris_mesh_resource_data_h__
