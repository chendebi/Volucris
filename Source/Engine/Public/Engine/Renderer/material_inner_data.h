#ifndef __volucris_material_inner_data_h__
#define __volucris_material_inner_data_h__

#include <Engine/Core/types_help.h>

namespace volucris
{
	enum MaterialUniformBlock
	{
		PRIMITIVE_INFO = 0x01,
		CAMERA_INFO = 0x02,
		DIRECTION_LIGHT = 0x04
	};

	using MaterialUniformBlocks = uint32;
}

#endif // !__volucris_material_inner_data_h__
