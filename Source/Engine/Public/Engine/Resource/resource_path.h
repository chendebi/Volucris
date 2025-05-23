#ifndef __volucris_resource_path_h__
#define __volucris_resource_path_h__

#include <string>

namespace volucris
{
	struct ResourcePath
	{
		std::string path;
		std::string name;
		std::string type;
		std::string fullpath;

		ResourcePath() = default;

		ResourcePath(const std::string& resoucePath);

		bool isValid() const
		{
			return !path.empty();
		}

		std::string getSystemPath() const;

		static bool SystemPathToResourcePath(const std::string& filepath, std::string& path);

		static bool ResourcePathToSystemPath(const std::string& filepath, std::string& path);
	};
}

#endif // !__volucris_resource_path_h__
