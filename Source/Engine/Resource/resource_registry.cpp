#include "Resource/resource_registry.h"
#include "Core/directory.h"
#include <Core/types_help.h>
#include "Core/volucris.h"
#include <filesystem>
#include <Core/assert.h>
#include <Resource/asset_reader.h>
#include <Resource/asset_writer.h>

namespace fs = std::filesystem;

namespace volucris
{

	struct ResourceHeader
	{
		char magic[8];
		int version;
	};

	ResourceRegistry::~ResourceRegistry()
	{
		
	}

	void ResourceRegistry::scanResources(const std::string& path)
	{
		std::string syspath;
		if (!getSystemPathByResourcePath(path, syspath))
		{
			V_LOG_WARN(Engine, "scan resource from {} failed.", path);
			return;
		}

		scanResourcesBySystemPath(syspath);
	}

	void ResourceRegistry::scanResourcesBySystemPath(const std::string& path)
	{
		
	}

	void ResourceRegistry::addResourceSearchPath(const std::string& systemPath, const std::string& header)
	{
		m_searchPaths.insert({ header, systemPath });
	}

	bool ResourceRegistry::getResourcePathBySystemPath(const std::string& systemPath, std::string& path)
	{
		fs::path canonicalFilePath = fs::weakly_canonical(systemPath);
		for (const auto& [header, sysPath] : m_searchPaths)
		{
			fs::path canonicalDir = fs::weakly_canonical(sysPath);
			if (canonicalFilePath.string().find(canonicalDir.string()) == 0) {
				// 获取相对于目录A的相对路径
				auto relativePath = fs::relative(canonicalFilePath, canonicalDir).generic_string();

				if (relativePath.compare(0, 2, "./") == 0)
				{
					relativePath = relativePath.substr(2);
				}
				else if (relativePath.compare(0, 1, ".") == 0)
				{
					relativePath = "";
				}

				if (!relativePath.empty())
				{
					path = header + "/" + relativePath;
				}
				else
				{
					path = header;
				}
				return true;
			}
		}
		return false;
	}

	bool ResourceRegistry::getSystemPathByResourcePath(const std::string& resPath, std::string& path)
	{
		for (const auto& [header, sysPath] : m_searchPaths)
		{
			fs::path canonicalDir = fs::weakly_canonical(sysPath);
			if (resPath.find(header) == 0) {
				fs::path relativePath = fs::relative(resPath, header);
				path = fs::absolute(sysPath + "/" + relativePath.generic_string()).string();
				return true;
			}
		}
		return false;
	}

	std::shared_ptr<ResourceObject> ResourceRegistry::loadResourceByAssetPath(const std::string& path)
	{
		auto it = m_caches.find(path);
		if (it != m_caches.end())
		{
			if (auto asset = it->second.lock())
			{
				return asset;
			}
			m_caches.erase(it);
		}

		std::string sysPath;
		if (!getSystemPathByResourcePath(path, sysPath))
		{
			V_LOG_WARN(Engine, "load asset failed. {}", path);
			return nullptr;
		}

		AssetReader reader;
		reader.setLoadPath(sysPath);
		auto object = reader.load();
		if (object && object->getAsset().getAssetPath() == path && object->getAsset().uuid.valid())
		{
			m_assets.insert_or_assign(object->getAsset().uuid, path);
			m_caches.insert({ path, object });
			return object;
		}
		else
		{
			V_LOG_WARN(Engine, "asset load success, but object invalid. {}", path);
		}
		return nullptr;
	}

	std::shared_ptr<ResourceObject> ResourceRegistry::loadResourceByAsset(const Asset& asset)
	{
		const auto assetPath = asset.getAssetPath();
		return loadResourceByAssetPath(assetPath);
	}

	Asset ResourceRegistry::getAsset(const std::string& path)
	{
		auto it = m_caches.find(path);
		if (it != m_caches.end())
		{
			if (auto asset = it->second.lock())
			{
				return asset->getAsset();
			}
		}

		std::string sysPath;
		if (!getSystemPathByResourcePath(path, sysPath))
		{
			V_LOG_WARN(Engine, "get asset failed. {}", path);
			return {};
		}

		AssetReader reader;
		reader.setLoadPath(sysPath);
		return reader.loadAsset();
	}

	bool ResourceRegistry::registry(const std::shared_ptr<ResourceObject>& resource)
	{
		auto asset = resource->getAsset();
		if (asset.type == AssetType::UNKNOWN)
		{
			V_LOG_WARN(Engine, "registry asset failed. asset type not supported");
			return false;
		}

		auto uuid = asset.uuid;
		if (uuid.valid() && m_assets.find(uuid) != m_assets.end())
		{
			V_LOG_WARN(Engine, "asset alredy registered.");
			return true;
		}

		asset.uuid = UUID::generate();
		resource->setAsset(asset);

		return true;
	}

	void ResourceRegistry::save(const std::shared_ptr<ResourceObject>& resource)
	{
		if (!resource->isDirty())
		{
			return;
		}

		AssetWriter writer;
		if (!writer.save(resource))
		{
			V_LOG_WARN(Engine, "save asset failed.");
		}
		else
		{
			V_LOG_WARN(Engine, "save asset success.");
		}
	}

	ResourceRegistry::ResourceRegistry()
	{
		m_assets.clear();
	}
}
