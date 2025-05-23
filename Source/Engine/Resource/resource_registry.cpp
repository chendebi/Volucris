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
				fs::path relativePath = fs::relative(canonicalFilePath, canonicalDir);
				// 转换为/Engine/开头的路径
				path = header + relativePath.generic_string();
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

	void ResourceRegistry::loadManifiset()
	{
#if WITH_EDITOR
		const auto filepath = Directory::EngineDirectory().absolute() / "Config/manifiset.json";
#endif
		const auto manifisetPath = filepath.toString();
		rapidjson::Document doc;
		FILE* fp = fopen(manifisetPath.c_str(), "r");
		if (!fp)
		{
			V_LOG_WARN(Engine, "load asset manifiset from {} failed", manifisetPath);
			return;
		}
		fseek(fp, 0, SEEK_END);
		size_t size = _ftelli64(fp);
		fseek(fp, 0, SEEK_SET);
		std::vector<uint8> buffer;
		buffer.resize(size+4);
		rapidjson::FileReadStream is(fp, (char*)buffer.data(), size);
		doc.ParseStream(is);
		fclose(fp);

		for (auto it = doc.MemberBegin(); it != doc.MemberEnd(); ++it)
		{
			std::string guid = std::string(it->name.GetString());
			std::string path = std::string(it->value.GetString());
			m_assets.insert({ GUID(guid), path});
			m_assetsSortByPath.insert({ path, GUID(guid) });
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
		rapidjson::Document doc;
		FILE* fp = fopen(sysPath.c_str(), "r");
		if (!fp)
		{
			V_LOG_WARN(Engine, "load asset {} failed", resPath);
			return nullptr;
		}
		fseek(fp, 0, SEEK_END);
		size_t size = _ftelli64(fp);
		fseek(fp, 0, SEEK_SET);
		std::vector<uint8> buffer;
		buffer.resize(size + 4);
		rapidjson::FileReadStream is(fp, (char*)buffer.data(), size);
		doc.ParseStream(is);
		fclose(fp);
		
		std::vector<std::shared_ptr<ResourceObject>> dependObjects;

		ResourceMeta meta;
		if (!getJsonValueString(doc, "guid", meta.guid))
		{
			V_LOG_WARN(Engine, "parse asset {} failed", resPath);
			return nullptr;
		}
		int type = 0;
		if (!getJsonValueInt(doc, "type", type))
		{
			V_LOG_WARN(Engine, "parse asset {} failed", resPath);
			return nullptr;
		}
		if (type < 0 || type > 2)
		{
			V_LOG_WARN(Engine, "parse asset {} failed", resPath);
			return nullptr;
		}

		meta.type = (ResourceType)type;
		std::vector<std::string> depends;
		if (!getJsonValueStringList(doc, "dependencies", depends))
		{
			V_LOG_WARN(Engine, "parse asset {} failed", resPath);
			return nullptr;
		}

		for (const auto guid : depends)
		{
			if (auto depend = loadResourceByGUID(GUID(guid)))
			{
				dependObjects.push_back(depend);
			}
			else
			{
				V_LOG_WARN(Engine, "load dependence {} failed.", guid);
			}
		}

		rapidjson::Value content;
		if (!getJsonValueObject(doc, doc.GetAllocator(), "content", content))
		{
			V_LOG_WARN(Engine, "parse asset {} failed", resPath);
			return nullptr;
		}

		return loadResource(meta, content, doc.GetAllocator());
	}

	bool ResourceRegistry::registry(const const std::shared_ptr<ResourceObject>& resource)
	{
		if (m_assets.find(GUID(resource->m_metaData.guid)) != m_assets.end())
		{
			V_LOG_WARN(Engine, "resource alredy registryed");
			return true;
		}
		ResourceMeta& meta = resource->m_metaData;
		auto id = GUID::generate();
		meta.guid = id.uuid;
		if (dynamic_cast<Material*>(resource.get()))
		{
			meta.type = ResourceType::MATERIAL;
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
		if (!registry(resource))
		{
			return;
		}
		rapidjson::Document doc;
		doc.SetObject(); // 设置为对象类型  

		// 获取分配器（必须用于添加成员）  
		auto& allocator = doc.GetAllocator();

		doc.AddMember(rapidjson::StringRef("guid"), rapidjson::StringRef(resource->m_metaData.guid.c_str()), allocator);
		doc.AddMember(rapidjson::StringRef("type"), (int)resource->m_metaData.type, allocator);
		doc.AddMember(rapidjson::StringRef("path"), rapidjson::StringRef(resource->m_metaData.path.c_str()), allocator);
		doc.AddMember(rapidjson::StringRef("resource"), rapidjson::StringRef(resource->m_metaData.sourcePath.c_str()), allocator);
		
		rapidjson::Value content(rapidjson::kObjectType);
		resource->serialize(content, allocator);
		doc.AddMember(rapidjson::StringRef("content"), content, allocator);

		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		doc.Accept(writer);

		std::ofstream ofs(resource->m_path.getSystemPath());
		if (ofs.is_open()) {
			ofs << buffer.GetString();
			ofs.close();
			V_LOG_INFO(Engine, "resource save success");
		}
		else {
			V_LOG_WARN(Engine, "resource save failed");
		}
	}

	std::shared_ptr<ResourceObject> ResourceRegistry::loadResource(const ResourceMeta& meta, const rapidjson::Value& serializer, rapidjson::Document::AllocatorType& allocator)
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
			object->deserialize(serializer, allocator);
			m_caches.insert({ meta.guid, object });
		}
		return object;
	}

	ResourceRegistry::ResourceRegistry()
	{
		m_assets.clear();
	}
}
