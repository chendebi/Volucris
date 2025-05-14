#ifndef __volucris_mesh_render_data_h__
#define __volucris_mesh_render_data_h__

#include <vector>
#include <Engine/Core/types_help.h>
#include <unordered_map>

namespace volucris
{
	class MeshResourceData;
	class MaterialProxy;

	enum class DrawMode
	{
		TRIANGLES
	};

	// type默认都是unsigned int
	struct SectionRenderData
	{
		DrawMode mode = DrawMode::TRIANGLES;
		int count = 0;
		int offset = 0;
	};

	enum BlockType
	{
		VERTEX,
		NORMAL
	};

	struct BlockDescription
	{
		BlockType type;
		size_t offset;
	};

	class MeshRenderData
	{
	public:
		MeshRenderData() = default;

		std::vector<BlockDescription> blocks;
		std::vector<uint8> renderData;
		std::vector<uint8> sectionData;
		std::unordered_map<MaterialProxy*, std::vector<SectionRenderData>> sections;
	};
}

#endif // !__volucris_mesh_render_data_h__
