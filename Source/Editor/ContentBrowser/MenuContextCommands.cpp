#include "MenuContextCommands.h"
#include "ContentBrowser/ContentItemWidget.h"
#include "ContentWidget.h"
#include "AssetMenuContext.h"

namespace volucris
{
	MenuContextCommands::MenuContextCommands()
		: m_context(nullptr)
	{
	}

	MenuContextCommands::MenuContextCommands(ItemContext* context)
		: m_context(context)
	{
	}

	RenameCommand::RenameCommand(ItemContext* context)
		: MenuContextCommands(context)
	{
	}

	void RenameCommand::execute()
	{
		m_context->getItemWidget()->setEditing(true);
	}

	DeleteFolderCommand::DeleteFolderCommand(FolderContext* context)
		: MenuContextCommands(context)
	{

	}

	void DeleteFolderCommand::execute()
	{
		const auto& path = m_context->getFullPath();
		const auto& assets = gAssetTool.getAssetsInfoInFolder(path, true);
		for (const auto& asset : assets)
		{
			AssetManager::getInstance().unregister(asset.data.path);
		}

		gFileSystem.deleteDirectory(path);
	}

	DeleteAssetCommand::DeleteAssetCommand(AssetContext* context)
		: MenuContextCommands(context)
	{
	}

	void DeleteAssetCommand::execute()
	{
		AssetManager::getInstance().unregister(m_context->getFullPath());
	}
}
