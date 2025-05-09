#include "Core/directory.h"
#include <Windows.h>
#include <filesystem>

namespace fs = std::filesystem;

namespace volucris
{
	static std::string AppDirectoryPath;

	Directory::Directory(const std::string& dirpath)
		: m_path(dirpath)
	{
	}

	Directory Directory::ApplicationDirectory()
	{
		if (AppDirectoryPath.empty())
		{
			char buffer[MAX_PATH] = { 0 };
			GetModuleFileNameA(NULL, buffer, MAX_PATH);
			std::string::size_type pos = std::string(buffer).find_last_of("\\/");
			AppDirectoryPath = std::string(buffer).substr(0, pos);
		}
		return AppDirectoryPath;
	}

	Directory Directory::EngineDirectory()
	{
		return VOLUCRIS_ENGINE_ROOT;
	}

	Directory Directory::absolute() const
	{
		return fs::absolute(m_path).string();
	}
}
