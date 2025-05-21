#include <string>
#ifndef __volucris_meta_data_h__
#define __volucris_meta_data_h__

namespace volucris
{
	enum ResourceType
	{
		MATERIAL = 1,
		STATIC_MESH
	};

	struct ResourceMeta
	{
		ResourceType type;
		std::string guid;
		std::string path;
		std::string sourcePath;
	};
}

#endif // !__volucris_meta_data_h__
