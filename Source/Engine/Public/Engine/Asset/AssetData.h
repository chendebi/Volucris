#ifndef __volucris_asset_data_h__
#define __volucris_asset_data_h__

#include <string>
#include <Engine/Core/GUID.h>
#include <vector>

namespace volucris
{
	struct AssetData
	{
		std::string path;
		std::string className;
		GUID guid;
		std::vector<std::string> tags;
		std::vector<std::string> dependencies;

		template <class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& path;
			ar& className;
			ar& guid;
			ar& tags;
			ar& dependencies;
		}
	};
}

#endif // !__volucris_asset_data_h__
