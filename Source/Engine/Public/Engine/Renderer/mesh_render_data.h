#ifndef __volucris_mesh_render_data_h__
#define __volucris_mesh_render_data_h__

#include <vector>
#include <Engine/Core/types_help.h>

namespace volucris
{
	class MeshResourceData;

	enum class DrawMode
	{
		TRIANGLES
	};

	// type默认都是unsigned int
	struct SectionRenderData
	{
		DrawMode mode;
		int count;
		int offset;
	};

	class MeshRenderData
	{
	public:
		MeshRenderData() = default;

		void build(MeshResourceData* data);

	private:
		friend class MeshResourceData;
		std::vector<uint8> m_renderData;
		std::vector<uint8> m_sectionData;
		std::vector<SectionRenderData> m_sections;
	};
}

#endif // !__volucris_mesh_render_data_h__
