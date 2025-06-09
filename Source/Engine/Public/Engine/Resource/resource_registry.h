#ifndef __volucris_resource_registry_h__
#define __volucris_resource_registry_h__

#include <memory>
#include <string>
#include <map>
#include <fstream>
#include "Engine/Resource/resource_object.h"
#include <Engine/Core/event.h>

namespace volucris
{
	class ResourceObject;

	DECLARE_EVENT(OnAssetRegistryed, std::shared_ptr<ResourceObject>)

	class ResourceRegistry
	{
	public:
		OnAssetRegistryed AssetRegistryed; // 资源注册事件

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

		std::shared_ptr<ResourceObject> loadResourceByAssetPath(const std::string& path);

		std::shared_ptr<ResourceObject> loadResourceByAsset(const Asset& asset);

		Asset getAsset(const std::string& path);

		template<typename T>
		std::shared_ptr<T> loadResource(const std::string& path)
		{
			return std::dynamic_pointer_cast<T>(loadResourceByAssetPath(path));
		}

		bool registry(const std::shared_ptr<ResourceObject>& resource);

		void save(const std::shared_ptr<ResourceObject>& resource);

	private:
		std::map<UUID, std::string> m_assets; // uuid对应的资产路径
		std::map<std::string, std::weak_ptr<ResourceObject>> m_caches; // 资产路径对应的资源对象
		std::map<std::string, std::string> m_searchPaths; // 扫描的资源路径

	protected:
		ResourceRegistry();
		ResourceRegistry(const ResourceRegistry&) = delete;
		ResourceRegistry& operator=(const ResourceRegistry&) = delete;
	};
}

#endif // !__volucris_resource_registry_h__
