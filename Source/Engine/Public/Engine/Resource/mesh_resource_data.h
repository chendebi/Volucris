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
		int count;
		int offset;
		std::string slot;

		Section() : mode(DrawMode::TRIANGLES), count(0), offset(0), slot()
		{
			
		}

		Section(DrawMode drawMode, int idxCount, int off, const std::string& matSlot)
			: mode(drawMode), count(idxCount), offset(off), slot(matSlot)
		{

		}
	};

	class MeshResourceData
	{
	public:
		MeshResourceData();

		void setVertices(void* vertices, size_t size);

		void addSection(const std::vector<uint32>& indices, const std::string& slot, DrawMode mode = DrawMode::TRIANGLES);

		void addSection(const Section& section);

		Section addSectionData(const std::vector<uint32>& indices);
		
		Section addSectionData(const uint32* indices, size_t count);

		std::shared_ptr<MeshRenderData> build();

		const std::vector<Section>& getSections() const { return m_sections; }

		std::shared_ptr<Material> getMaterial(const std::string& slot) const
		{
			auto it = m_materials.find(slot);
			if (it == m_materials.end())
			{
				return nullptr;
			}
			return it->second;
		}

		void setMaterial(const std::string& slot, const std::shared_ptr<Material>& material)
		{
			m_materials[slot] = material;
		}

		const std::unordered_map<std::string, std::shared_ptr<Material>>& getMaterials() const { return m_materials; }

	private:
		std::vector<glm::vec3> m_vertices;
		std::vector<uint32> m_indices;
		std::vector<Section> m_sections;
		std::unordered_map<std::string, std::shared_ptr<Material>> m_materials;
	};
}

#endif // !__volucris_mesh_resource_data_h__
