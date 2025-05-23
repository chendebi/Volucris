#include "Resource/resource_path.h"
#include <filesystem>
#include "Core/volucris.h"
#include <Core/directory.h>
#include "Resource/resource_registry.h"

namespace fs = std::filesystem;

namespace volucris
{
	ResourcePath::ResourcePath(const std::string& resoucePath)
	{
		fullpath = resoucePath;
		auto rpath = fs::path(resoucePath);
		path = rpath.parent_path().string();
		name = rpath.stem().string();
		auto ext = rpath.extension().string();
		if (!ext.empty())
		{
			type = ext.substr(1);
		}
	}

	std::string ResourcePath::getSystemPath() const
	{
		std::string path;
		ResourceRegistry::Instance().getSystemPathByResourcePath(fullpath, path);
		return path;
	}

	bool ResourcePath::SystemPathToResourcePath(const std::string& filepath, std::string& path)
	{
		return ResourceRegistry::Instance().getResourcePathBySystemPath(filepath, path);
	}

	bool ResourcePath::ResourcePathToSystemPath(const std::string& filepath, std::string& path)
	{
		return ResourceRegistry::Instance().getSystemPathByResourcePath(filepath, path);
	}
}