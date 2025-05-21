#ifndef __volucris_resource_registry_h__
#define __volucris_resource_registry_h__

#include <memory>
#include <string>
#include <unordered_map>
#include "Engine/Resource/resource_object.h"
#include "meta_data.h"
#include <xhash>

namespace volucris
{
	struct GUID
	{
		::std::string uuid;

		GUID() : uuid() {}

		explicit GUID(const ::std::string& id)
			: uuid(id)
		{
		}

		bool operator==(const GUID& other) const
		{
			return other.uuid == uuid;
		}
	};
}

namespace std
{
	template<>
	struct hash<volucris::GUID>
	{
		inline size_t operator()(const volucris::GUID& val) const
		{
			return std::hash<std::string>{}(val.uuid);
		}
	};
}

namespace volucris
{

	class ResourceRegistry
	{
	public:
		~ResourceRegistry();

		static ResourceRegistry& Instance()
		{
			static ResourceRegistry inst;
			return inst;
		}

		void loadManifiset();

		std::shared_ptr<ResourceObject> loadResourceByGUID(const GUID& guid);

		std::shared_ptr<ResourceObject> loadResourceByPath(const std::string& path);

		template<typename T>
		std::shared_ptr<T> loadResource(const GUID& guid)
		{
			return std::dynamic_pointer_cast<T>(loadResourceByGUID(guid));
		}

	protected:
		std::shared_ptr<ResourceObject> loadResource(const ResourceMeta& meta, const rapidjson::Value& serializer, rapidjson::Document::AllocatorType& allocator);

	private:
		std::unordered_map <std::string, std::weak_ptr<ResourceObject>> m_caches;
		std::unordered_map<GUID, std::string> m_assets;
		std::unordered_map<std::string, GUID> m_assetsSortByPath;

	protected:
		ResourceRegistry();

		ResourceRegistry(const ResourceRegistry&) = delete;
		ResourceRegistry& operator=(const ResourceRegistry&) = delete;
	};
}

#endif // !__volucris_resource_registry_h__
