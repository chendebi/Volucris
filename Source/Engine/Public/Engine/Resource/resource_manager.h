#ifndef __volucris_resource_manager_h__
#define __volucris_resource_manager_h__

#include <memory>
#include <string>
#include <map>
#include "Engine/Resource/resource_object.h"

namespace volucris
{
	class Material;

	class ResourceManager
	{
	public:
		~ResourceManager();

		static ResourceManager* Instance()
		{
			static ResourceManager inst;
			return &inst;
		}

		std::shared_ptr<ResourceObject> getResourceObject(const std::string& assetPath);

		template<typename T>
		std::shared_ptr<T> getResource(const std::string& assetPath)
		{
			auto object = getResourceObject(assetPath);
			return std::dynamic_pointer_cast<T>(object);
		}

	protected:
		std::shared_ptr<Material> loadMaterial(const ResourcePath& path);

	private:
		std::map<std::string, std::shared_ptr<ResourceObject>> m_resources;

	protected:
		ResourceManager();

		ResourceManager(const ResourceManager&) = delete;
		ResourceManager(ResourceManager&&) = delete;
		ResourceManager& operator=(const ResourceManager&) = delete;
		ResourceManager& operator=(ResourceManager&&) = delete;
	};
}

#define gResources volucris::ResourceManager::Instance()

#endif // !__volucris_resource_manager_h__
