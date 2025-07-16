#include <Asset/AssetManager.h>
#include <Game/Package.h>
#include <Core/Volucris.h>
#include <Asset/AssetWriter.h>
#include <Asset/AssetReader.h>
#include <Game/World.h>

namespace volucris
{
	AssetManager::AssetManager()
	{
		// 初始化代码
	}

	bool AssetManager::registry(Package* package)
	{
		if (m_assetDatas.find(package->getAssetData().path) != m_assetDatas.end()) {
			V_LOG_WARN(Engine, "Package {} is already registered.", package->getAssetData().path);
			return false; // 如果包已经注册，则返回false
		}

		auto packageName = package->getAssetData().path;
		m_packages[packageName] = package->getShared<Package>();
		return true;
	}

	void AssetManager::unregister(const std::string& packageName)
	{
		
	}

	void AssetManager::updateAssetData(const std::string& packageName, const AssetData& assetData)
	{
		auto it = m_assetDatas.find(packageName);
		if (it != m_assetDatas.end()) {
			it->second = assetData; // 更新现有数据
		} else {
			V_LOG_WARN(Engine, "Asset data for package {} not found.", packageName);
		}
		
		// todo: 触发事件通知
		//m_packageRegisteredEvent.invoke(packageName);
	}

	void AssetManager::save(Package* package)
	{
		AssetWriter writer = AssetWriter(package->getShared<Package>());
		writer.write();
	}

	std::shared_ptr<Package> AssetManager::load(const std::string& packageName, World* world)
	{
		{
			auto it = m_packages.find(packageName);
			if (it != m_packages.end() && !it->second.expired()) {
				return it->second.lock(); // 如果包已注册且未过期，则返回
			}
		}
		
		AssetReader reader = AssetReader(packageName);
		auto package = reader.readPackage();
		if (package)
		{
			m_packages[packageName] = package;
			if (world)
			{
				world->addPackage(package);
			}
		}
		return package;
	}
}