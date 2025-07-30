#include <Common/AssetTool.h>
#include <EditorCore/Editor.h>
#include <Engine/Asset/AssetWriter.h>
#include <Engine/Game/Material.h>
#include <Engine/Game/MaterialParameter.h>
#include <Engine/Render/MaterialParameterInfo.h>
#include <Engine/Game/MaterialInstance.h>
#include "AssetObjectHelper.h"
#include <Engine/FileSystem/FileSystem.h>

namespace volucris
{
	AssetTool::AssetTool()
		: m_dirtyAssets()
	{
		AssetManager::getInstance().AssetRegistered.bindObject(this, &AssetTool::onAssetRegistered);
		AssetManager::getInstance().AssetUnregistered.bindObject(this, &AssetTool::onAssetUnregistered);
	}

	bool AssetTool::save(const std::shared_ptr<Package>& package)
	{
		if (!package)
		{
			return false;
		}

		if (package->getAssetObject() == nullptr)
		{
			V_LOG_ERROR(Editor, "AssetTool::save: Package has no asset object set.");
			return false;
		}

		if (auto helper = AssetObjectHelper::getAssetHelper(package->getAssetObject()))
		{
			auto assetData = package->getAssetData();
			assetData.dependencies = helper->getDependences();
			package->setAssetData(assetData);
		}

		AssetWriter writer = AssetWriter(std::shared_ptr<Package>(package));
		
		if (writer.write())
		{
			V_LOG_INFO(Editor, "AssetTool::save: Package saved successfully: {}", package->getAssetData().path);
			removeDirtyAsset(package->getAssetData().path);
			return true;
		}
		else
		{
			V_LOG_ERROR(Editor, "AssetTool::save: Failed to save package: {}", package->getAssetData().path);
		}
		return false;
	}

	void AssetTool::addDirtyAsset(const std::string& packageName, const std::shared_ptr<GameObject>& object)
	{
		if (!object || packageName.empty())
		{
			V_LOG_WARN(Editor, "AssetTool: Invalid dirty object.");
			return;
		}

		if (!AssetManager::getInstance().isPackageRegistered(packageName))
		{
			V_LOG_WARN(Editor, "AssetTool: Package not registered: {}", packageName);
			return;
		}

		m_dirtyAssets[packageName] = object;
		AssetInfo info;
		info.data = AssetManager::getInstance().loadAssetData(packageName);
		info.object = object;
		info.dirty = true;
		AssetDirtyStateChanged.invoke(info);
	}

	void AssetTool::renamePackage(const std::shared_ptr<Package>& package, const std::string& newPackageName)
	{
		auto& inst = AssetManager::getInstance();
		auto assetData = package->getAssetData();

		if (!package || newPackageName.empty() || !inst.isPackageRegistered(assetData.path))
		{
			V_LOG_WARN(Editor, "AssetTool: Invalid package or new package name.");
			return;
		}

		inst.unregister(package->getAssetData().path);
		
		assetData.path = newPackageName;
		package->setAssetData(assetData);
		inst.registry(package.get());

		const auto packageNames = inst.getReferenceAssets(package->getAssetData().path);
		for (const auto& packageName : packageNames)
		{
			if (auto object = inst.load(packageName))
			{
				auto helper = AssetObjectHelper::getAssetHelper(object);
				if (helper->updateDependences({ { package->getAssetData().path, newPackageName } }))
				{
					addDirtyAsset(packageName, object);
				}
			}
		}
	}

	std::vector<AssetInfo> AssetTool::getAssetsInfoInFolder(const std::string& folder, bool recursion) const
	{
		std::vector<AssetInfo> infos;
		const auto assetDatas = AssetManager::getInstance().getAssetsInDirectory(folder, recursion);
		for (const auto& assetData : assetDatas)
		{
			AssetInfo info;
			info.data = assetData;
			info.object = AssetManager::getInstance().tryLoad(assetData.path);
			info.dirty = m_dirtyAssets.find(assetData.path) != m_dirtyAssets.end();
			infos.push_back(info);
		}
		return infos;
	}

	void AssetTool::removeDirtyAsset(const std::string& packageName)
	{
		auto it = m_dirtyAssets.find(packageName);
		if (it == m_dirtyAssets.end())
		{
			return;
		}
		AssetInfo info;
		info.data = AssetManager::getInstance().loadAssetData(packageName);
		info.object = it->second;
		info.dirty = false;
		m_dirtyAssets.erase(it);
		AssetDirtyStateChanged.invoke(info);
	}

	void AssetTool::onAssetRegistered(Package* package)
	{
		const auto& packageName = package->getAssetData().path;
		if (m_dirtyAssets.find(packageName) != m_dirtyAssets.end())
		{
			V_LOG_WARN(Editor, "AssetTool: Package {} is already registered as dirty.", packageName);
			return;
		}
		addDirtyAsset(packageName, package->getAssetObject());
	}

	void AssetTool::onAssetUnregistered(const std::string& packageName)
	{
		auto& inst = AssetManager::getInstance();
		const auto packageNames = inst.getReferenceAssets(packageName);
		for (const auto& packageName : packageNames)
		{
			if (auto object = inst.load(packageName))
			{
				auto helper = AssetObjectHelper::getAssetHelper(object);
				if (helper->updateDependences({ { packageName, ""}}))
				{
					addDirtyAsset(packageName, object);
				}
			}
		}
		m_dirtyAssets.erase(packageName);

		gFileSystem.deleteAsset(packageName);

		AssetDeleted.invoke(packageName);
	}
}