#include "ContentBrowerWidget.h"
#include <imgui/imgui.h>
#include <imgui_internal.h>
#include "AssetListWidget.h"
#include "PathTreeWidget.h"
//#include <Engine/Resource/resource_path.h>
#include <Engine/FileSystem/FileSystem.h>

namespace volucris
{
	ContentBrowserWidget::ContentBrowserWidget()
		: Widget()
		, m_assetWidget(std::make_shared<AssetListWidget>())
		, m_pathWidget(std::make_shared<PathTreeWidget>())
	{

		// 建立事件连接
		m_pathWidget->connectToDirectoryEvent(
			m_assetWidget->getDirectorySelectedEvent()
		);

		//ResourcePath::ResourcePathToSystemPath("/Engine/", engineContentPath);
		std::string engineContentPath = FileSystem::getInstance().virtualToPhysical("/Engine/Content/");
		m_pathWidget->addRootPath(engineContentPath, "Engine");
	}

	void ContentBrowserWidget::onBuild()
	{
		//ImGui::Begin("Content Browser");
		ImGui::Begin("Content Browser", nullptr, ImGuiWindowFlags_None); // 允许调整大小

		m_pathWidget->onBuild();

		// 分割条
		ImGui::SameLine();
		ImGui::InvisibleButton("Splitter", ImVec2(8.0f, ImGui::GetContentRegionAvail().y + 0.1));
		if (ImGui::IsItemActive() && ImGui::IsMouseDragging(0)) {
			auto width = m_pathWidget->getWidth() + ImGui::GetIO().MouseDelta.x;
			width = ImMax(width, 50.0f); // 最小宽度限制
			m_pathWidget->setWidth(width);
		}
		if (ImGui::IsItemHovered() || (ImGui::IsItemActive() && is_dragging)) {
			ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
			is_dragging = ImGui::IsItemActive();
		}

		// 右边面板
		ImGui::SameLine();
		
		m_assetWidget->onBuild();
		ImGui::End();

	}
}