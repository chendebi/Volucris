#ifndef __volucris_mesh_resource_data_h__
#define __volucris_mesh_resource_data_h__

#include <glm/glm.hpp>
#include <vector>
#include <Engine/Core/TypesHelp.h>
#include <Engine/Core/PrimitiveInfo.h>

namespace volucris
{
	struct SmallSubMesh
	{
		std::vector<uint8> indices;
		ElementDrawMode drawMode = ElementDrawMode::Traingles;

		size_t getIndexDataSize() const
		{
			return indices.size();
		}
	};

	struct MediumSubMesh
	{
		std::vector<uint16> indices;
		ElementDrawMode drawMode = ElementDrawMode::Traingles;

		size_t getIndexDataSize() const
		{
			return sizeof(uint16) * indices.size();
		}
	};

	struct LargeSubMesh
	{
		std::vector<uint32> indices;
		ElementDrawMode drawMode = ElementDrawMode::Traingles;

		size_t getIndexDataSize() const
		{
			return sizeof(uint32) * indices.size();
		}
	};

	class MeshData
	{
		std::vector<glm::vec3> m_vertices;
		std::vector<glm::vec3> m_normals;
		std::vector<glm::vec3> m_uv0;
		std::vector<glm::vec3> m_uv1;
		std::vector<glm::vec4> m_color;
		std::vector<glm::vec4> m_secondColor;

		std::vector<uint8> m_submeshData;
		std::vector<PrimitiveSegment> m_segments;

	public:
		void setVertices(std::vector<glm::vec3> vertices)
		{
			m_vertices = std::move(vertices);
		}

		void setNormals(std::vector<glm::vec3> normals)
		{
			m_normals = std::move(normals);
		}

		void setColor(std::vector<glm::vec4> colors)
		{
			m_color = std::move(colors);
		}

		void addSubMesh(SmallSubMesh mesh);

		void addSubMesh(MediumSubMesh mesh);

		void addSubMesh(LargeSubMesh mesh);

		void reserveSubMeshData(size_t size)
		{
			m_submeshData.reserve(size);
		}
	};
}

#endif // !__volucris_mesh_resource_data_h__
