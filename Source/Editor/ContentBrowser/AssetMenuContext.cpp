#include "AssetMenuContext.h"
#include "ContentWidget.h"
#include <imgui_internal.h>

namespace volucris
{
	void ItemContext::clicked()
	{
		m_contentWidget->setSelectedItem(m_itemWidget);
	}

	bool ItemContext::buildMenuContext()
	{
		if (ImGui::BeginPopupContextItem())
		{
			for (const auto& group : m_menuGroups)
			{
				if (!group.name.empty())
				{
					ImGui::TextUnformatted(group.name.c_str());
					ImGui::Separator();
				}
				for (const auto& item : group.items)
				{
					if (ImGui::MenuItem(item.name.c_str()))
					{
						item.command->execute();
					}
				}
			}
			ImGui::EndPopup();
			return true;
		}
		return false;
	}

	FolderContext::FolderContext(ContentWidget* contentWidget, ContentItemWidget* itemWidget)
		: ItemContext(contentWidget, itemWidget)
	{
		init();
	}

	void FolderContext::rename(const std::string& newName)
	{
		
	}

	void FolderContext::doubleClicked()
	{
		m_contentWidget->dirtyCurrentFolder(getFullPath());
	}

	void FolderContext::init()
	{
		{
			MenuContextGroup group;
			group.name = "";
			{
				MenuContextItem item;
				item.name = "Rename";
				item.command = std::make_unique<RenameCommand>(this);
				group.items.push_back(std::move(item));
			}
			{
				MenuContextItem item;
				item.name = "Delete";
				item.command = std::make_unique<DeleteFolderCommand>(this);
				group.items.push_back(std::move(item));
			}
			addMenuContextGroup(std::move(group));
		}
	}

	void AssetContext::rename(const std::string& newName)
	{

	}

	void AssetContext::addAssetMenuContexts()
	{
		MenuContextGroup group;
		{
			MenuContextItem item;
			item.name = "Rename";
			item.command = std::make_unique<RenameCommand>(this);
			group.items.push_back(std::move(item));
		}
		{
			MenuContextItem item;
			item.name = "Delete";
			item.command = std::make_unique<DeleteAssetCommand>(this);
			group.items.push_back(std::move(item));
		}
		addMenuContextGroup(std::move(group));
	}

	void MaterialContext::addMaterialMenuContexts()
	{
		{
			{
				auto group = getMenuContextGroup(0);
				MenuContextItem item;
				item.name = "Create Instance";
				item.command = std::make_unique<RenameCommand>(this);
				group->items.insert(group->items.begin(), std::move(item));
			}
			MenuContextGroup group;
			group.name = "Material";
			{
				MenuContextItem item;
				item.name = "Reload From Source";
				item.command = std::make_unique<DeleteAssetCommand>(this);
				group.items.push_back(std::move(item));
			}
			addMenuContextGroup(std::move(group));
		}
	}
	void MaterialInstanceContext::addMaterialInstanceMenuContexts()
	{
		{
			auto group = getMenuContextGroup(0);
			{
				MenuContextItem item;
				item.name = "Open Editor";
				item.command = std::make_unique<RenameCommand>(this);
				group->items.insert(group->items.begin(), std::move(item));
			}

			{
				MenuContextItem item;
				item.name = "Create Instance";
				item.command = std::make_unique<RenameCommand>(this);
				group->items.insert(group->items.begin(), std::move(item));
			}
		}
	}
}