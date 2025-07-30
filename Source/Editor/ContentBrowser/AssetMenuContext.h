#ifndef __volucris_asset_menu_context_h__
#define __volucris_asset_menu_context_h__

#include <Common/AssetTool.h>
#include <Engine/Asset/AssetPath.h>
#include "MenuContextCommands.h"
#include <Common/Thumbnail.h>

namespace volucris
{
	class ContentWidget;
	class ContentItemWidget;

	struct MenuContextItem
	{
		std::string name;
		std::unique_ptr<MenuContextCommands> command;
	};

	struct MenuContextGroup
	{
		std::string name;
		std::vector<MenuContextItem> items;
	};

	class ItemContext
	{
	public:
		ItemContext(ContentWidget* contentWidget, ContentItemWidget* itemWidget)
			: m_dirty(false)
			, m_assetPath("")
			, m_displayName()
			, m_thumbnail()
			, m_menuGroups()
			, m_contentWidget(contentWidget)
			, m_itemWidget(itemWidget)
		{
		}

		virtual ~ItemContext() = default;

		const std::string& getAssetName() const
		{
			return m_assetPath.name;
		}

		const std::string& getDisplayName() const
		{
			return m_displayName;
		}

		const std::string& getFullPath() const { return m_assetPath.fullpath; }

		void setDirty(bool dirty)
		{
			m_dirty = dirty;
			updateDisplayName();
		}

		virtual void clicked();

		virtual void doubleClicked() {}

		void setAssetPath(AssetPath assetPath)
		{
			m_assetPath = std::move(assetPath);
			updateDisplayName();
		}

		ContentWidget* getContentWidget() const
		{
			return m_contentWidget;
		}

		ContentItemWidget* getItemWidget() const
		{
			return m_itemWidget;
		}

		void setThumbnail(const Thumbnail& thumbnail)
		{
			m_thumbnail = thumbnail;
		}

		Thumbnail& getThumbnail()
		{
			return m_thumbnail;
		}

		void setDisplayName(const std::string& name)
		{
			m_displayName = name;
			m_dirty = false;
		}

		virtual void rename(const std::string& newName) = 0;

		bool buildMenuContext();

		void addMenuContextGroup(MenuContextGroup group)
		{
			m_menuGroups.push_back(std::move(group));
		}

	protected:
		MenuContextGroup* getMenuContextGroup(size_t index)
		{
			if (index < m_menuGroups.size())
			{
				return &m_menuGroups[index];
			}
			return nullptr;
		}

	private:
		void updateDisplayName()
		{
			m_displayName = m_dirty ? fmt::format("*{}", m_assetPath.name) : m_assetPath.name;
		}

	private:
		bool m_dirty;
		AssetPath m_assetPath;
		std::string m_displayName;
		Thumbnail m_thumbnail;
		std::vector<MenuContextGroup> m_menuGroups;

	protected:
		ContentWidget* m_contentWidget;
		ContentItemWidget* m_itemWidget;
	};

	class FolderContext : public ItemContext
	{
	public:
		FolderContext(ContentWidget* contentWidget, ContentItemWidget* itemWidget);

		void rename(const std::string& newName) override;

		void doubleClicked() override;

	private:
		void init();
	};

	

	class AssetContext : public ItemContext
	{
	public:
		AssetContext(ContentWidget* contentWidget, ContentItemWidget* itemWidget)
			: ItemContext(contentWidget, itemWidget)
			, m_assetInfo()
		{
			addAssetMenuContexts();
		}

		void setAssetInfo(const AssetInfo& info)
		{
			setAssetPath(m_assetInfo.data.path);
		}

		void rename(const std::string& newName) override;

	private:
		void addAssetMenuContexts();

	private:
		AssetInfo m_assetInfo;
	};

	class MaterialContext : public AssetContext
	{
	public:
		MaterialContext(ContentWidget* contentWidget, ContentItemWidget* itemWidget)
			: AssetContext(contentWidget, itemWidget)
		{
			addMaterialMenuContexts();
		}

	private:
		void addMaterialMenuContexts();
	};

	class MaterialInstanceContext : public AssetContext
	{
	public:
		MaterialInstanceContext(ContentWidget* contentWidget, ContentItemWidget* itemWidget)
			: AssetContext(contentWidget, itemWidget)
		{
			addMaterialInstanceMenuContexts();
		}

	private:
		void addMaterialInstanceMenuContexts();
	};
}

#endif // !__volucris_asset_menu_context_h__
