#include "content_browser_widget.h"
#include <imgui/imgui.h>
#include <imgui_internal.h>
#include "asset_list_widget.h"
#include "path_tree_widget.h"
#include <Engine/Resource/resource_path.h>

namespace volucris
{
	ContentBrowserWidget::ContentBrowserWidget()
		: Widget()
		, m_assetWidget(std::make_shared<AssetListWidget>())
		, m_pathWidget(std::make_shared<PathTreeWidget>())
	{
		std::string engineContentPath;
		ResourcePath::ResourcePathToSystemPath("/Engine/", engineContentPath);
		m_pathWidget->addRootPath(engineContentPath, "Engine");
	}

	void ContentBrowserWidget::build()
	{
		ImGui::Begin("Content Browser");
		m_pathWidget->build();

		// 分割条
		ImGui::SameLine();
		ImGui::InvisibleButton("Splitter", ImVec2(8.0f, ImGui::GetContentRegionAvail().y + 0.1));
		if (ImGui::IsItemActive() && ImGui::IsMouseDragging(0)) {
			auto width = m_pathWidget ->getWidth() + ImGui::GetIO().MouseDelta.x;
			width = ImMax(width, 50.0f); // 最小宽度限制
			m_pathWidget->setWidth(width);
		}
		if (ImGui::IsItemHovered() || (ImGui::IsItemActive() && is_dragging)) {
			ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
			is_dragging = ImGui::IsItemActive();
		}

		// 右边面板
		ImGui::SameLine();
		
		m_assetWidget->build();
		ImGui::End();

	}
}