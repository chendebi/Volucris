#include "Resource/asset_path.h"
#include <filesystem>
#include <Core/volucris.h>
#include <guiddef.h>
#include <combaseapi.h>
#include <Resource/asset_reader.h>
#include <Resource/resource_registry.h>

namespace fs = std::filesystem;

namespace volucris
{
	UUID UUID::generate()
	{
		UUID id;
		CoInitialize(NULL);
		_GUID guid;
		CoCreateGuid(&guid);

		// 将 GUID 格式化为字符串
		wchar_t guidStr[40];
		StringFromGUID2(guid, guidStr, sizeof(guidStr) / sizeof(guidStr[0]));

		int size_needed = WideCharToMultiByte(CP_UTF8, 0, guidStr, -1, NULL, 0, NULL, NULL);
		std::string str(size_needed, 0);
		WideCharToMultiByte(CP_UTF8, 0, guidStr, -1, &str[0], size_needed, NULL, NULL);
		str.pop_back();
		id.value = str;

		CoUninitialize();
		return id;
	}

	Asset::Asset(const std::string& pathName)
		: uuid()
		, path()
		, name()
		, sourcePath()
	{
		auto rpath = fs::path(pathName);
		path = rpath.parent_path().string();
		name = rpath.stem().string();
		assetPath = fmt::format("{}/{}", path, name);
	}

	std::string Asset::getAssetPath() const
	{
		return assetPath;
	}

	std::shared_ptr<ResourceObject> Asset::load()
	{
		return ResourceRegistry::Instance().loadResourceByAsset(*this);
	}
}