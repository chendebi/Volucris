#ifndef __volucris_asset_path_h__
#define __volucris_asset_path_h__

#include <string>
#include <filesystem>

namespace fs = std::filesystem;

namespace volucris
{
	struct AssetPath
	{
		std::string fullpath;
		std::string path;
		std::string name;

		AssetPath(const std::string& packageName)
			: fullpath(packageName)
		{
			auto packagePath = fs::path(packageName);
			path = packagePath.parent_path().generic_u8string();
			name = packagePath.stem().generic_u8string();
		}
	};
}

#endif // !__volucris_asset_path_h__
