#include "Resource/resource_registry.h"
#include "Core/directory.h"
#include <rapidjson/filereadstream.h>
#include <rapidjson/filewritestream.h>
#include <rapidjson/writer.h>
#include <rapidjson/document.h>
#include <Core/types_help.h>
#include "Core/volucris.h"
#include <Resource/material.h>
#include <Resource/meta_data.h>
#include <fstream>
#include <xhash>
#include <combaseapi.h>
#include <filesystem>
#include <Core/assert.h>
#include <Core/serializer.h>
#include <Resource/static_mesh.h>

namespace fs = std::filesystem;

namespace volucris
{
	GUID GUID::generate()
	{
		GUID id;
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
		id.uuid = str;

		CoUninitialize();
		return id;
	}

	struct ResourceHeader
	{
		char magic[8];
		int version;
	};

	ResourceRegistry::~ResourceRegistry()
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

	void ResourceRegistry::updateResourcePath(ResourceObject* resource, const std::string& newPath)
	{
		if (resource && resource->m_metaData.isValid())
		{
			const auto guid = GUID(resource->m_metaData.guid);
			auto it = m_assets.find(guid);
			if (it == m_assets.end())
			{
				V_LOG_WARN(Engine, "asset not registered, path: {}", resource->m_metaData.path);
				return;
			}
			it->second = newPath;
			m_assetsSortByPath.erase(resource->m_metaData.path);
			m_assetsSortByPath.insert({ newPath,  guid});
			resource->m_metaData.path = newPath;
			resource->m_path = newPath;
		}
	}

	std::shared_ptr<ResourceObject> ResourceRegistry::loadResourceByGUID(const GUID& guid)
	{
		auto it = m_assets.find(guid);
		if (it == m_assets.end())
		{
			return nullptr;
		}

		// 尝试打开文件
		return loadResourceByPath(it->second);
	}

	std::shared_ptr<ResourceObject> ResourceRegistry::loadResourceByPath(const std::string& resPath)
	{
		ResourcePath path = ResourcePath(resPath);
		auto sysPath = path.getSystemPath();

		std::ifstream fin = std::ifstream(sysPath, std::ios::binary);
		if (!fin.is_open())
		{
			V_LOG_WARN(Engine, "load resource from {} failed.", resPath);
			return nullptr;
		}

		ResourceHeader header;
		fin.read((char*)&header, sizeof(header));
		if (fin.gcount() < 4)
		{
			V_LOG_WARN(Engine, "load resource from {} failed, file corrupted.", resPath);
			return nullptr;
		}

		if (std::string(header.magic, 8) != "volucris")
		{
			V_LOG_WARN(Engine, "load resource from {} failed, file not a asset.", resPath);
			return nullptr;
		}

		ResourceMeta meta = readResourceMeta(fin);

		if (!meta.isValid())
		{
			V_LOG_WARN(Engine, "load resource from {} failed, file corrupted.", resPath);
			return nullptr;
		}

		// 读取资源内容
		uint32 contentSize = 0;
		fin.read((char*)&contentSize, sizeof(uint32));
		if (fin.gcount() < sizeof(uint32))
		{
			V_LOG_WARN(Engine, "load resource from {} failed, file corrupted.", resPath);
			return nullptr;
		}

		std::vector<uint8> contentData(contentSize);
		fin.read((char*)contentData.data(), contentSize);
		if (fin.gcount() < contentSize)
		{
			V_LOG_WARN(Engine, "load resource from {} failed, file corrupted.", resPath);
			return nullptr;
		}

		Serializer serializer;
		serializer.setData(std::move(contentData));
		
		return loadResource(meta, serializer);
	}

	ResourceMeta ResourceRegistry::getResourceMeta(const std::string& resPath)
	{
		ResourcePath path = ResourcePath(resPath);
		auto sysPath = path.getSystemPath();

		std::ifstream fin = std::ifstream(sysPath, std::ios::binary);
		if (!fin.is_open())
		{
			V_LOG_WARN(Engine, "load resource from {} failed.", resPath);
			return {};
		}

		return readResourceMeta(fin);
	}

	bool ResourceRegistry::registry(const const std::shared_ptr<ResourceObject>& resource, const std::string& path)
	{
		if (m_assets.find(GUID(resource->m_metaData.guid)) != m_assets.end())
		{
			V_LOG_WARN(Engine, "resource alredy registryed");
			return true;
		}
		ResourceMeta& meta = resource->m_metaData;
		auto id = GUID::generate();
		meta.guid = id.uuid;
		meta.path = path;
		resource->setResourcePath(path);
		if (dynamic_cast<Material*>(resource.get()))
		{
			meta.type = ResourceType::MATERIAL;
		}
		else if (dynamic_cast<StaticMesh*>(resource.get()))
		{
			meta.type = ResourceType::STATIC_MESH;
		}
		else
		{
			V_LOG_WARN(Engine, "unsupported resource");
			return false;
		}
		m_assets.insert({ id, resource->m_path.fullpath });
		m_assetsSortByPath.insert({ resource->m_path.fullpath, id });
		return true;
	}

	void ResourceRegistry::save(const std::shared_ptr<ResourceObject>& resource)
	{
		if (!resource->m_metaData.isValid())
		{
			return;
		}

		Serializer resourceSerializer;
		if (!resource->serialize(resourceSerializer))
		{
			return;
		}

		ResourceHeader header = { {'v','o','l','u','c','r', 'i', 's'}, 1 };
		std::ofstream fout = std::ofstream(resource->m_path.getSystemPath(), std::ios::binary | std::ios::trunc);
		fout.write((char*)&header, sizeof(header));
		if (fout.fail())
		{
			V_LOG_WARN(Engine, "save resource to {} failed.", resource->m_path.fullpath);
			return;
		}

		{
			Serializer metaDataSerializer;
			metaDataSerializer.serialize(resource->m_metaData);
			uint32 size = metaDataSerializer.getData().size();
			fout.write((char*)&size, sizeof(uint32));
			fout.write((char*)metaDataSerializer.getData().data(), metaDataSerializer.getData().size());
		}

		uint32 size = resourceSerializer.getData().size();
		fout.write((char*)&size, sizeof(uint32));
		fout.write((char*)resourceSerializer.getData().data(), resourceSerializer.getData().size());
		

		if (fout.fail())
		{
			V_LOG_WARN(Engine, "save resource to {} failed.", resource->m_path.fullpath);
			return;
		}
		fout.close();
	}

	bool ResourceRegistry::makesureDependenceValid(const std::shared_ptr<ResourceObject>& resource)
	{
		if (!resource)
		{
			return false;
		}

		if (!resource->getMetaData().isValid())
		{
			// 弹窗?
			check(false);
			return false;
		}

		return true;
	}

	void ResourceRegistry::serializeDependenceTo(Serializer& serializer, const std::shared_ptr<ResourceObject>& resource)
	{
		if (!resource || !resource->getMetaData().isValid())
		{
			serializer.serialize("");
		}
		else
		{
			serializer.serialize(resource->getMetaData().guid);
		}
	}

	std::shared_ptr<ResourceObject> ResourceRegistry::loadResource(const ResourceMeta& meta, Serializer& serializer)
	{
		std::shared_ptr<ResourceObject> object;
		switch (meta.type)
		{
		case ResourceType::MATERIAL:
			object = std::make_shared<Material>();
			break;
		case ResourceType::STATIC_MESH:
			break;
		default:
			break;
		}
		if (object)
		{
			object->m_metaData = meta;
			object->deserialize(serializer);
			m_caches.insert({ meta.guid, object });
		}
		return object;
	}

	ResourceMeta ResourceRegistry::readResourceMeta(std::ifstream& fin)
	{
		fin.seekg(0, std::ios::beg);
		ResourceHeader header;
		fin.read((char*)&header, sizeof(header));
		if (fin.gcount() < 4)
		{
			return {};
		}

		if (std::string(header.magic, 8) != "volucris")
		{
			return {};
		}

		uint32 metaSize;
		fin.read((char*)&metaSize, sizeof(uint32));
		if (fin.gcount() < sizeof(uint32))
		{
			return {};
		}

		std::vector<uint8> metaData;
		metaData.resize(metaSize);
		fin.read((char*)metaData.data(), metaSize);
		if (fin.gcount() < metaSize)
		{
			return {};
		}

		Serializer serializer;
		serializer.setData(std::move(metaData));

		ResourceMeta meta;
		serializer.deserialize(meta);
		return meta;
	}

	ResourceRegistry::ResourceRegistry()
	{
		m_assets.clear();
	}
}
