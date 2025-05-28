#ifndef __volucris_resource_registry_h__
#define __volucris_resource_registry_h__

#include <memory>
#include <string>
#include <map>
#include "Engine/Resource/resource_object.h"
#include "meta_data.h"
#include <xhash>
#include <fstream>

namespace volucris
{
	struct GUID
	{
		std::string uuid;

		GUID() : uuid() {}

		explicit GUID(const ::std::string& id)
			: uuid(id)
		{
		}

		bool operator==(const GUID& other) const
		{
			return other.uuid == uuid;
		}

		bool operator<(const GUID& other) const
		{
			return uuid < other.uuid;
		}

		static GUID generate();
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

		void scanResources(const std::string& path);

		void scanResourcesBySystemPath(const std::string& path);

		void addResourceSearchPath(const std::string& systemPath, const std::string& header);

		bool getResourcePathBySystemPath(const std::string& systemPath, std::string& path);

		bool getSystemPathByResourcePath(const std::string& resPath, std::string& path);

		void updateResourcePath(ResourceObject* resource, const std::string& newPath);

		std::shared_ptr<ResourceObject> loadResourceByGUID(const GUID& guid);

		std::shared_ptr<ResourceObject> loadResourceByPath(const std::string& path);

		ResourceMeta getResourceMeta(const std::string& path);

		template<typename T>
		std::shared_ptr<T> loadResource(const GUID& guid)
		{
			return std::dynamic_pointer_cast<T>(loadResourceByGUID(guid));
		}

		template<typename T>
		std::shared_ptr<T> loadResource(const std::string& path)
		{
			return std::dynamic_pointer_cast<T>(loadResourceByPath(path));
		}

		bool registry(const std::shared_ptr<ResourceObject>& resource, const std::string& path);

		void save(const std::shared_ptr<ResourceObject>& resource);

		bool makesureDependenceValid(const std::shared_ptr<ResourceObject>& resource);

		void serializeDependenceTo(Serializer& serializer, const std::shared_ptr<ResourceObject>& resource);

	protected:
		std::shared_ptr<ResourceObject> loadResource(const ResourceMeta& meta, Serializer& serializer);

		ResourceMeta getResourceMetaBySystemPath(const std::string& path);

		ResourceMeta readResourceMeta(std::ifstream& fin);

	private:
		std::map <std::string, std::weak_ptr<ResourceObject>> m_caches;
		std::map<GUID, std::string> m_assets;
		std::map<std::string, GUID> m_assetsSortByPath;

		std::map<std::string, std::string> m_searchPaths;

	protected:
		ResourceRegistry();
		ResourceRegistry(const ResourceRegistry&) = delete;
		ResourceRegistry& operator=(const ResourceRegistry&) = delete;
	};
}

#endif // !__volucris_resource_registry_h__
