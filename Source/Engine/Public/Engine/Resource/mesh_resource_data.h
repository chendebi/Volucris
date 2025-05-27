#ifndef __volucris_mesh_resource_data_h__
#define __volucris_mesh_resource_data_h__

#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <memory>
#include <unordered_map>
#include <Engine/Core/types_help.h>
#include "Engine/Renderer/mesh_render_data.h"
#include <Engine/Core/serializer.h>

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

		void serialize(Serializer& serializer)
		{
			serializer.serialize(mode);
			serializer.serialize(count);
			serializer.serialize(offset);
			serializer.serialize(slot);
		}

		void deserialize(Serializer& serializer)
		{
			serializer.deserialize(mode);
			serializer.deserialize(count);
			serializer.deserialize(offset);
			serializer.deserialize(slot);
		}
	};

	class MeshResourceData
	{
	public:
		MeshResourceData();

		void setVertices(void* vertices, size_t size);

		void setVertices(std::vector<glm::vec3> vertices);

		void setNormals(std::vector<glm::vec3> normals);

		void setUV(int idx, std::vector<glm::vec3> uv);

		void addSection(const std::vector<uint32>& indices, const std::string& slot, DrawMode mode = DrawMode::TRIANGLES);

		void addSection(const Section& section);

		Section addSectionData(const std::vector<uint32>& indices);
		
		Section addSectionData(const uint32* indices, size_t count);

		std::shared_ptr<MeshRenderData> build();

		const std::vector<Section>& getSections() const { return m_sections; }

		void serialize(Serializer& serializer);

		void deserialize(Serializer& serializer);

	private:
		std::vector<glm::vec3> m_vertices;
		std::vector<glm::vec3> m_normals;
		std::vector<std::vector<glm::vec3>> m_uvs;
		std::vector<uint32> m_indices;
		std::vector<Section> m_sections;
	};
}

#endif // !__volucris_mesh_resource_data_h__
