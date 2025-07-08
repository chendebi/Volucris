#ifndef __volucris_asset_manager_h__
#define __volucris_asset_manager_h__

#include <Engine/Game/GameObject.h>
#include <map>
#include "AssetData.h"
#include <Engine/Core/Delegate.h>

namespace volucris
{
	DECLARE_EVENT_MUTI_DELEGATE(PackageRegistered, void, const std::string&)

	class Package;

	class AssetManager
	{
	public:
		~AssetManager() = default;

		static AssetManager& getInstance()
		{
			static AssetManager inst;
			return inst;
		}

		bool registry(Package* package);

		void unregister(const std::string& packageName);

		void updateAssetData(const std::string& packageName, const AssetData& assetData);

		bool isPackageRegistered(const std::string& packageName) const
		{
			return m_packages.find(packageName) != m_packages.end();
		}

		void save(Package* package);

		std::shared_ptr<Package> load(const std::string& packageName);

		template<typename T>
		std::shared_ptr<T> loadAsset(const std::string& packageName)
		{
			if (auto package = load(packageName))
			{
				for (const auto& child : package->getChildren())
				{
					if (auto asset = std::dynamic_pointer_cast<T>(child))
					{
						asset->setParent(nullptr);
						return asset;
					}
				}
			}
			return nullptr;
		}

	private:
		AssetManager();

		AssetManager(const AssetManager&) = delete;
		AssetManager(AssetManager&&) = delete;
		AssetManager& operator=(const AssetManager&) = delete;
		AssetManager& operator=(AssetManager&&) = delete;
		
	private:
		std::map<std::string, std::weak_ptr<Package>> m_packages;
		std::map<std::string, AssetData> m_assetDatas;
	};
}

#endif // !__volucris_asset_manager_h__
