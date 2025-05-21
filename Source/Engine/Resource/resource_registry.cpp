#include "Resource/resource_registry.h"
#include "Core/directory.h"
#include <rapidjson/filereadstream.h>
#include <rapidjson/document.h>
#include <Core/types_help.h>
#include "Core/volucris.h"
#include <Resource/material.h>
#include <Resource/meta_data.h>
#include <xhash>

namespace volucris
{
	ResourceRegistry::~ResourceRegistry()
	{

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
