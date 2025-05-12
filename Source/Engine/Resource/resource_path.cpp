#include "Resource/resource_path.h"
#include <filesystem>
#include "Core/volucris.h"
#include <Core/directory.h>

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

	std::string ResourcePath::getSystemPath(const std::string& customExt) const
	{
		std::string ext = customExt.empty() ? type : customExt;

		Directory dir;
		if (path.substr(0, 7) == "/Engine")
		{
			dir = Directory::EngineDirectory() / path.substr(7);
		}
		else if (path.substr(0, 8) == "/Project")
		{
			dir = Directory::ApplicationDirectory() / path.substr(8);
		}
		else
		{
			dir = Directory::ApplicationDirectory() / path;
		}

		auto filepath = (dir / name).absolute().toString();
		return filepath + "." + ext;
	}
}