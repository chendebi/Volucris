#include "asset_list_widget.h"
#include <imgui/imgui.h>
#include "path_tree_widget.h"
#include <iostream>
#include <filesystem>


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
	AssetListWidget::AssetListWidget()
		: m_thumbnailSize(128.0f)
		, m_assetColor(glm::vec4(0.2f, 0.2f, 0.2f, 1.0f))
		, m_currentFiles()
	{
		m_pathSelectedEvent.addObject(this, &AssetListWidget::setCurrentPath);
		
		//m_eventToken = m_pathSelectedEvent.addLambda(
		//	[this](const std::string& path) {
		//		setCurrentPath(path);
		//	});

	}

	AssetListWidget::~AssetListWidget()
	{
		m_pathSelectedEvent.removeAll(this);
	}

	void AssetListWidget::build()
	{
		ImGui::BeginChild("RightPane", ImVec2(0, 0), true);

		//ImGui::Text("Right Content");
		renderAssetGrid();
		ImGui::EndChild();
	}

	void AssetListWidget::setCurrentFiles()
	{
		m_currentFiles.clear();
		try {
			for (const auto& entry : fs::directory_iterator(m_currentPath.c_str())) {
				if (entry.is_regular_file()) {
					std::string filename = entry.path().filename().string();
					if (entry.path().extension().empty()) {  // 无扩展名
						m_currentFiles.push_back(filename);
					}
				}
			}
		}
		catch (...) {
			// 处理可能的权限错误等
		}
	}

	void AssetListWidget::renderAssetGrid()
	{
		if (!m_currentPath.empty())
		{
			ImGui::Text("Files in: %s", m_currentPath.c_str());
			ImGui::Separator();

			// 获取可用宽度并计算列数
			float windowWidth = ImGui::GetContentRegionAvail().x;
			int columns = (int)(windowWidth / (m_thumbnailSize + ImGui::GetStyle().ItemSpacing.x));
			columns = columns > 0 ? columns : 1;

			ImGui::Columns(columns, nullptr, false);

			for (const auto& file : m_currentFiles) {

				// 使用文件路径作为唯一ID前缀
				ImGui::PushID(file.c_str());

				ImGui::BeginGroup();

				// 绘制矩形框
				ImVec4 ImassetColor = ImVec4(m_assetColor.x, m_assetColor.y, m_assetColor.z, m_assetColor.w);
				ImGui::PushStyleColor(ImGuiCol_Button, ImassetColor);
				if (ImGui::Button("##FileThumbnail", ImVec2(m_thumbnailSize, m_thumbnailSize))) {
					// #TODO:文件点击处理
				}
				ImGui::PopStyleColor();

				// 在矩形框内显示"Material"文字
				ImVec2 textSize = ImGui::CalcTextSize("Material");
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (m_thumbnailSize - textSize.x) * 0.5f);
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() - m_thumbnailSize + (m_thumbnailSize - textSize.y) * 0.5f);
				ImGui::Text("Material");

				// 显示文件名（下方居中）
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + (m_thumbnailSize - textSize.y) * 0.5f);
				std::string displayName = file.length() > 15 ?
					file.substr(0, 12) + "..." :  file;
				GUIHelpers::TextCentered(displayName.c_str(), m_thumbnailSize);

				ImGui::EndGroup();

				// 工具提示显示完整文件名
				if (ImGui::IsItemHovered()) {
					ImGui::BeginTooltip();
					ImGui::Text("%s", file.c_str());
					ImGui::EndTooltip();
				}


				ImGui::PopID(); // 结束唯一ID作用域
				ImGui::NextColumn();
			}

			ImGui::Columns(1);
		}
		else {
			ImGui::Text("Click on a directory to view files");
		}
	}
}
