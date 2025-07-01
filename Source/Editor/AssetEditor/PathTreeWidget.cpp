#include "PathTreeWidget.h"
#include <imgui/imgui.h>
#include <fmt/format.h>
#include <iostream>
#include <filesystem>
#include <imgui_internal.h>
#include "AddResourceWidget.h"
//#include <Engine/Resource/resource_path.h>
#include "AssetListWidget.h"
//#include <EditorEntry/editor_core.h>

namespace fs = std::filesystem;

namespace GUIHelpers {
	inline void TextCentered(const char* text, float width) {
		float textWidth = ImGui::CalcTextSize(text).x;
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (width - textWidth) * 0.5f);
		ImGui::Text("%s", text);
	}
}

namespace volucris
{
	DirectoryItem::DirectoryItem(PathTreeWidget* manager, const std::string& fullPath)
		: Widget()
		, m_selected(false)
		, m_newPopOpened(false)
		, m_isRightClick(false)
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

	void DirectoryItem::onBuild()
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

	void DirectoryItem::refresh()
	{
		m_subItems.clear();
		for (const auto& entry : fs::directory_iterator(m_directoryPath)) {
			if (entry.is_directory()) {
				m_subItems.push_back(DirectoryItem(m_manager, entry.path().string()));
			}
		}
	}

	void DirectoryItem::addChild(const std::string& dirName)
	{
		try {
			fs::path newPath = fs::path(m_directoryPath) / dirName;
			if (!fs::exists(newPath))
			{


				//children.push_back(newDir);

				//sortChildren();
			}

			//不存在则将数据同步到DirectoryItem中

			//refresh();
		}
		catch (...) {
			// 处理创建目录错误
		}
	}

	std::string DirectoryItem::getResourceDirectory() const
	{
		std::string path;
		//ResourcePath::SystemPathToResourcePath(m_directoryPath, path);
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
			result = ImGui::TreeNodeEx(m_displayName.c_str(), m_isRightClick ? ImGuiTreeNodeFlags_Selected : 0);
			//ImGui::Text(m_displayName.c_str());
			//ImGui::TreePop();

		}

		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		float parent_width = ImGui::GetWindowWidth();
		// 获取节点矩形区域
		ImRect rect = ImGui::GetCurrentContext()->LastItemData.Rect;
		rect.Max.x = parent_width;

		if (!m_selected && (ImGui::IsMouseClicked(ImGuiMouseButton_Left) || ImGui::IsMouseClicked(ImGuiMouseButton_Right))) {
			ImVec2 mousePos = ImGui::GetMousePos();
			if (rect.Contains(mousePos)) {
				m_selected = true;
				m_manager->setSelectedItem(this);
			}
		}

		if (m_selected)
		{
			if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
				m_manager->showPopWindow(ImGui::IsItemHovered());
				m_isRightClick = true;
			}

			// 绘制自定义背景
			draw_list->AddRectFilled(rect.Min, rect.Max, IM_COL32(50, 50, 150, 100));

			// 点击节点时加载文件列表
			if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {

				if (m_manager->getSelectedEvent())
				{
					(*(m_manager->getSelectedEvent()))(m_manager->getSelectedPathItem()->getDirectoryPath()); // 触发事件
				}
			}
		}
		else
		{
			m_isRightClick = false;
		}


		return result;
	}

	PathTreeWidget::PathTreeWidget()
		: Widget()
		, m_width(300.f)
		, m_showAddResourceWidget(false)
		, m_rootItems()
		, m_popOpened(false)
		, m_selectedItem(nullptr)
		, m_currentAction(ContextMenuAction::None)
		, m_pendingNewDialog(false)
		, m_showActionDialog(false)
		, m_showConfirmDialog(false)
		, m_dialogPos(glm::vec2(-FLT_MAX, -FLT_MAX))
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

	void PathTreeWidget::onBuild()
	{
		ImGui::BeginChild("LeftPane", ImVec2(m_width, 0), true);

		if (ImGui::Button("add"))
		{
			m_addResourceDlg->setOpened();
		}
		ImGui::SameLine();

		if (ImGui::Button("new"))
		{
			m_currentAction = ContextMenuAction::New;
			strcpy(m_inputBuffer, "New Folder");
			m_pendingNewDialog = true;
			ImVec2 mousePos = ImGui::GetMousePos();
			m_dialogPos = glm::vec2(mousePos.x, mousePos.y);
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();

		if (ImGui::Button("delete"))
		{
			m_currentAction = ContextMenuAction::Delete;
			m_showConfirmDialog = true;
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();

		if (ImGui::Button("rename"))
		{
			m_currentAction = ContextMenuAction::Rename;
			strcpy(m_inputBuffer, m_selectedItem->getDisplayName().c_str());
			m_showActionDialog = true;
			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("refresh"))
		{
			if (m_selectedItem)
			m_selectedItem->refresh();
		}

		m_addResourceDlg->build();


		for (auto& item : m_rootItems)
		{
			item.onBuild();
		}

		if (m_popOpened)
		{
			ImGui::OpenPopup("my_context_menu");
			m_popOpened = false;
		}

		//if (ImGui::BeginPopupContextWindow("my_context_menu"))  //注释理由：不注释右键点击节点会崩溃
		//{
		//	return; //暂时不用弹出方式改用按钮的方式
		//
		//	if (ImGui::MenuItem("new"))
		//	{
		//		m_currentAction = ContextMenuAction::New;
		//		strcpy(m_inputBuffer, "New Folder");
		//		m_pendingNewDialog = true;
		//		ImVec2 mousePos = ImGui::GetMousePos();
		//		m_dialogPos = glm::vec2(mousePos.x, mousePos.y);
		//		ImGui::CloseCurrentPopup();
		//
		//		// 标记事件已处理
		//		ImGui::GetIO().MouseDown[0] = false;
		//	}
		//
		//	ImGui::Separator();
		//	if (ImGui::MenuItem("delete"))
		//	{
		//		m_currentAction = ContextMenuAction::Delete;
		//		m_showConfirmDialog = true;
		//		ImGui::CloseCurrentPopup();
		//	}
		//	ImGui::Separator();
		//	if (ImGui::MenuItem("rename"))
		//	{
		//		m_currentAction = ContextMenuAction::Rename;
		//		strcpy(m_inputBuffer, m_selectedItem->getDisplayName().c_str());
		//		m_showActionDialog = true;
		//		ImGui::CloseCurrentPopup();
		//	}
		//
		//	ImGui::EndPopup();
		//}

		// 处理新建对话框
		if (m_pendingNewDialog) {
			ImGui::OpenPopup("Create New Directory");
			m_pendingNewDialog = false;
			m_showActionDialog = true;
		}
		
		// 渲染对话框
		if (m_showActionDialog) {
			showActionDialog(m_selectedItem);
			//ImGui::GetIO().MouseDown[0] = false;   //防止事件继续向下传播
		}
		
		if (m_showConfirmDialog) {
			showConfirmDialogFunc(m_selectedItem);
		}
		
		if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
		{
			m_popOpened = false;
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

	void PathTreeWidget::showActionDialog(DirectoryItem* node)
	{
		if (!m_showActionDialog) return;

		const char* dialogTitle = "";
		const char* actionButton = "";

		switch (m_currentAction) {
		case ContextMenuAction::New:
			dialogTitle = "Create New Directory";
			actionButton = "Create";
			break;
		case ContextMenuAction::Rename:
			dialogTitle = "Rename Directory";
			actionButton = "Rename";
			break;
		default:
			return;
		}

		// 如果是第一次显示对话框，设置位置为鼠标位置
		if (m_dialogPos.x == -FLT_MAX) {
			ImVec2 mousePos = ImGui::GetMousePos();
			m_dialogPos = glm::vec2(mousePos.x, mousePos.y);
		}

		float height = 0;

		// 始终尝试打开模态窗口
		if (ImGui::BeginPopupModal(dialogTitle, NULL, ImGuiWindowFlags_AlwaysAutoResize)) {

			height = ImGui::GetWindowHeight();

			ImGui::Text("Name:");
			ImGui::InputText("##NameInput", m_inputBuffer, sizeof(m_inputBuffer));

			if (ImGui::Button(actionButton)) {
				performFileSystemAction(node);
				m_showActionDialog = false;
				m_dialogPos = glm::vec2(-FLT_MAX, -FLT_MAX); // 重置位置
				ImGui::CloseCurrentPopup();
				// 标记事件已处理
			}

			ImGui::SameLine();
			if (ImGui::Button("Cancel")) {
				m_showActionDialog = false;
				m_dialogPos = glm::vec2(-FLT_MAX, -FLT_MAX); // 重置位置
				ImGui::CloseCurrentPopup();
				// 标记事件已处理
				ImGui::GetIO().MouseDown[0] = false;
			}

			ImGui::EndPopup();
		}
		else {
			// 如果窗口还没打开，就打开它
			ImGui::OpenPopup(dialogTitle);
		}


		// 设置下一个窗口的位置
		ImGui::SetNextWindowPos(ImVec2(m_dialogPos.x, m_dialogPos.y - height), ImGuiCond_Appearing);
	}

	void PathTreeWidget::showConfirmDialogFunc(DirectoryItem* node)
	{
		if (!m_showConfirmDialog) return;

		// 如果是第一次显示对话框，设置位置为鼠标位置
		if (m_dialogPos.x == -FLT_MAX) {
			ImVec2 mousePos = ImGui::GetMousePos();
			m_dialogPos = glm::vec2(mousePos.x, mousePos.y);
		}

		float height = 0;

		// 始终尝试打开模态窗口
		if (ImGui::BeginPopupModal("Confirm Delete", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
			ImGui::Text("Are you sure you want to delete '%s'?", node->getDisplayName().c_str());

			height = ImGui::GetWindowHeight();

			if (ImGui::Button("Yes")) {
				performFileSystemAction(node);
				m_showConfirmDialog = false;
				m_dialogPos = glm::vec2(-FLT_MAX, -FLT_MAX); // 重置位置
				ImGui::CloseCurrentPopup();
			}

			ImGui::SameLine();
			if (ImGui::Button("No")) {
				m_showConfirmDialog = false;
				m_dialogPos = glm::vec2(-FLT_MAX, -FLT_MAX); // 重置位置s
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
		else {
			// 如果窗口还没打开，就打开它
			ImGui::OpenPopup("Confirm Delete");
		}

		// 设置下一个窗口的位置
		ImGui::SetNextWindowPos(ImVec2(m_dialogPos.x, m_dialogPos.y - height), ImGuiCond_Appearing);
	}


	void  PathTreeWidget::performFileSystemAction(DirectoryItem* node)
	{
		try {
			switch (m_currentAction) {
			case ContextMenuAction::New: {
				fs::path newPath = fs::path(node->getDirectoryPath()) / m_inputBuffer;
				if (!fs::exists(newPath)) {
					fs::create_directory(newPath);
					node->addChild(m_inputBuffer);
					node->refresh();
				}
				break;
			}
			case ContextMenuAction::Delete:
			{
				fs::path pathToDelete = node->getDirectoryPath();
				if (fs::exists(pathToDelete))
				{
					// 找到父节点并移除当前节点
					//bool found = (std::find(m_rootItems.begin(), m_rootItems.end(), &node) != m_rootItems.end());
					auto it = std::find_if(m_rootItems.begin(), m_rootItems.end(),
						[&](const DirectoryItem& item) {
							return item.getDisplayName() == node->getDisplayName();
						});

					bool found = (it != m_rootItems.end());

					if (!found)
					{
						fs::path parentPath = fs::path(node->getDirectoryPath()).parent_path();
						fs::remove_all(pathToDelete);

						// 重新加载父节点的子目录
						m_rootItems[0].refresh();
						//DirectoryItem* parent = findParent(&m_rootItems[0], node);
						//if (parent)
						//	parent->refresh();
					}
				}
				break;
			}
			case ContextMenuAction::Rename:
			{
				fs::path oldPath = node->getDirectoryPath();
				fs::path newPath = oldPath.parent_path() / m_inputBuffer;
				if (fs::exists(oldPath) && !fs::exists(newPath)) {
					fs::rename(oldPath, newPath);
					node->setDisplayName(m_inputBuffer);
					node->setDirectoryPath(newPath.string());

					// 更新父节点的子目录列表
					m_rootItems[0].refresh();

				}
				break;
			}
			default:
				break;
			}
		}
		catch (const std::exception& e)
		{
			//V_LOG_ERROR(Editor, e.what())
		}
	}


	DirectoryItem* PathTreeWidget::findParent(DirectoryItem* root, DirectoryItem* target)
	{
		for (auto& child : root->getChild())
		{
			if (child.getDisplayName() == target->getDisplayName())
			{
				return root;
			}

			DirectoryItem* found = findParent(&child, target);
			if (found)
			{
				return found;
			}

		}

		return nullptr;
	}
}
