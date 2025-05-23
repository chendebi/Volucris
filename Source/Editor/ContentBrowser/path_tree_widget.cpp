#include "path_tree_widget.h"
#include <imgui/imgui.h>
#include <fmt/format.h>
#include <iostream>
#include <filesystem>
#include <imgui_internal.h>
#include "add_resource_widget.h"
#include <Engine/Resource/resource_path.h>

namespace fs = std::filesystem;

namespace volucris
{
	DirectoryItem::DirectoryItem(PathTreeWidget* manager, const std::string& fullPath)
		: Widget()
		, m_selected(false)
		, m_directoryPath(fullPath)
		, m_displayName()
		, m_manager(manager)
		, m_subItems()
	{
		auto path = fs::path(m_directoryPath);
		m_displayName = path.stem().string();

		for (const auto& entry : fs::directory_iterator(fullPath)) {
			if (entry.is_directory()) {
				m_subItems.push_back(DirectoryItem(manager, entry.path().string()));
			}
		}
		
	}

	void DirectoryItem::build()
	{

		if (buildItem())
		{
			for (auto& item : m_subItems)
			{
				item.build();
			}
			ImGui::TreePop();
		}
		
	}

	std::string DirectoryItem::getResourceDirectory() const
	{
		std::string path;
		ResourcePath::SystemPathToResourcePath(m_directoryPath, path);
		return path;
	}

	bool DirectoryItem::buildItem()
	{
		bool result = false;

		if (!m_subItems.empty())
		{
			result = ImGui::TreeNode(m_displayName.c_str());
		}
		else
		{
			ImGui::Text(m_displayName.c_str());
		}

		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		float parent_width = ImGui::GetWindowWidth();
		// 获取节点矩形区域
		ImRect rect = ImGui::GetCurrentContext()->LastItemData.Rect;
		rect.Max.x = parent_width;

		if (!m_selected && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
			ImVec2 mousePos = ImGui::GetMousePos();
			if (rect.Contains(mousePos)) {
				m_selected = true;
				m_manager->setSelectedItem(this);
			}
		}

		if (m_selected)
		{
			// 绘制自定义背景
			draw_list->AddRectFilled(rect.Min, rect.Max, IM_COL32(50, 50, 150, 100));
		}

		return result;
	}

	PathTreeWidget::PathTreeWidget()
		: Widget()
		, m_width(300.f)
		, m_showAddResourceWidget(false)
		, m_rootItems()
		, m_selectedItem(nullptr)
		, m_addResourceDlg(std::make_shared<AddResourceDialog>(this))
	{
	}

	void PathTreeWidget::addRootPath(const std::string& rootPath, const std::string& displayName)
	{
		auto item = DirectoryItem(this, rootPath);
		item.setDisplayName(displayName);
		m_rootItems.emplace_back(item);
		if (!m_selectedItem)
		{
			setSelectedItem(&m_rootItems[0]);
		}
	}

	void PathTreeWidget::build()
	{
		ImGui::BeginChild("LeftPane", ImVec2(m_width, 0), true);
		
		if (ImGui::Button("add"))
		{
			m_addResourceDlg->setOpened();
		}

		m_addResourceDlg->build();

		for (auto& item : m_rootItems)
		{
			item.build();
		}

		ImGui::EndChild();
	}
	void PathTreeWidget::setSelectedItem(DirectoryItem* item)
	{
		if (m_selectedItem)
		{
			m_selectedItem->setSelected(false);
		}
		m_selectedItem = item;
		m_selectedItem->setSelected(true);
	}
}
