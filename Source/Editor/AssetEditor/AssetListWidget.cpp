#include "AssetListWidget.h"
//#include <Engine/Resource/resource_registry.h>
//#include "Engine/Core/logging.h"
//#include <Engine/Resource/asset_reader.h>
#include <imgui/imgui.h>
#include "PathTreeWidget.h"
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
		//, m_assetObjects()
		, m_showAlreadySavedPopup(false)
	{
		//m_pathSelectedEvent = [this](const std::string& path) {
		//	this->setCurrentPath(path);
		//	};

		//ResourceRegistry::Instance().AssetRegistryed.addObject(this, &AssetListWidget::setAssetObject);
		//m_eventToken = m_pathSelectedEvent.addLambda(
		//	[this](const std::string& path) {
		//		setCurrentPath(path);
		//	});

	}

	AssetListWidget::~AssetListWidget()
	{
		//m_pathSelectedEvent.removeAll(this);
	}

	void AssetListWidget::onBuild()
	{
		ImGui::BeginChild("RightPane", ImVec2(0, 0), true);

		ImGui::Text("Right Content");
		renderAssetGrid();

		if (ImGui::BeginPopupModal("Object Already Saved")) {
			// 这个空Begin会实际渲染之前通过OpenPopup请求的窗口
			ImGui::EndPopup();
		}

		if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) || ImGui::IsKeyDown(ImGuiKey_RightCtrl)) {
			if (ImGui::IsKeyPressed(ImGuiKey_S)) {
				//// 查找当前选中的对象
				//auto selectedIt = std::find_if(m_assetObjects.begin(), m_assetObjects.end(),
				//	[](const std::shared_ptr<ResourceObject>& obj) { return obj->getPackage().pkIsSelect; });
				//
				//if (selectedIt != m_assetObjects.end())
				//{
				//
				//	auto assetPath = (*selectedIt)->getAsset().getAssetPath();
				//	std::string sysPath;
				//	if (!ResourceRegistry::Instance().getSystemPathByResourcePath(assetPath, sysPath))
				//	{
				//		//V_LOG_WARN(Engine, "save asset failed. asset path invalid, {}", assetPath);
				//		return;
				//	}
				//
				//	if (fs::exists(sysPath))
				//	{
				//		AssetReader reader;
				//		auto existed = reader.loadAsset();
				//		if (!existed.uuid.valid() || existed.uuid != (*selectedIt)->getAsset().uuid)
				//		{
				//			m_showAlreadySavedPopup = true;
				//		}
				//	}
				//
				//	ResourceRegistry::Instance().save(*selectedIt);
				//	(*selectedIt)->getPackage().pkDirty = true;
				//	m_assetObjects.erase(selectedIt);
				//}
			}
		}

		// 已保存提示弹窗
		//if (m_showAlreadySavedPopup) {
		//	ImGui::OpenPopup("Object Already Saved");
		//	m_showAlreadySavedPopup = false; // 重置状态
		//}
		//
		//// 实际渲染弹窗
		//if (ImGui::BeginPopupModal("Object Already Saved", nullptr,
		//	ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))
		//{
		//	ImGui::Text("This object has already been saved.");
		//	ImGui::Spacing();
		//	ImGui::Separator();
		//	ImGui::Spacing();
		//
		//	float buttonWidth = 120.0f;
		//	ImGui::SetCursorPosX((ImGui::GetWindowWidth() - buttonWidth) * 0.5f);
		//	if (ImGui::Button("OK", ImVec2(buttonWidth, 0))) {
		//		ImGui::CloseCurrentPopup();
		//	}
		//
		//	ImGui::EndPopup();
		//}

		ImGui::EndChild();
	}

	void AssetListWidget::setCurrentFiles(const std::string& path)
	{
		m_currentFiles.clear();
		try {
			for (const auto& entry : fs::directory_iterator(path.c_str())) {
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
		if (!m_currentPath.empty() /*|| !m_assetObjects.empty()*/)
		{
			ImGui::Text("Files in: %s", m_currentPath.c_str());
			ImGui::Separator();

			// 获取可用宽度并计算列数
			float windowWidth = ImGui::GetContentRegionAvail().x;
			int columns = (int)(windowWidth / (m_thumbnailSize + ImGui::GetStyle().ItemSpacing.x));
			columns = columns > 0 ? columns : 1;

			ImGui::Columns(columns, nullptr, false);

			// 绘制 m_currentFiles 的项
			for (const auto& file : m_currentFiles) {
				drawFileItem(file);
				ImGui::NextColumn();
			}

			// 绘制 m_object 的项
			//for (const auto& obj : m_assetObjects) {
			//	drawObjectItem(obj);
			//	ImGui::NextColumn();
			//}

			ImGui::Columns(1);
		}
		else {
			ImGui::Text("Click on a directory to view files");
		}
	}


	//void AssetListWidget::setAssetObject(std::shared_ptr<ResourceObject> res)
	//{
	//	if (!m_assetObjects.empty())
	//	{
	//		for (auto& item : m_assetObjects)
	//		{
	//			if (item->getAsset().uuid == res->getAsset().uuid)
	//				return;   //存在了就返回
	//			else
	//				m_assetObjects.push_back(res);//m_assetObjects.insert(std::make_pair(res, false));
	//		}
	//	}
	//	else
	//	{
	//		//m_assetObjects.insert(std::make_pair(res, false));
	//		m_assetObjects.push_back(res);

	//	}
	//}

	void AssetListWidget::drawFileItem(const std::string& file)
	{
		ImGui::PushID(file.c_str());
		ImGui::BeginGroup();

		// 绘制矩形框
		ImVec4 assetColor = ImVec4(m_assetColor.x, m_assetColor.y, m_assetColor.z, m_assetColor.w);
		ImGui::PushStyleColor(ImGuiCol_Button, assetColor);
		if (ImGui::Button("##FileThumbnail", ImVec2(m_thumbnailSize, m_thumbnailSize))) {
			//handleFileClick(file);
		}
		ImGui::PopStyleColor();

		// 在矩形框内显示"Material"文字
		ImVec2 textSize = ImGui::CalcTextSize("Material");
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (m_thumbnailSize - textSize.x) * 0.5f);
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() - m_thumbnailSize + (m_thumbnailSize - textSize.y) * 0.5f);
		ImGui::Text("Material");

		// 显示文件名（下方居中）
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + (m_thumbnailSize - textSize.y) * 0.5f);
		std::string displayName = file.length() > 15 ? file.substr(0, 12) + "..." : file;
		GUIHelpers::TextCentered(displayName.c_str(), m_thumbnailSize);

		ImGui::EndGroup();

		// 工具提示
		if (ImGui::IsItemHovered()) {
			ImGui::BeginTooltip();
			ImGui::Text("%s", file.c_str());
			ImGui::EndTooltip();
		}

		ImGui::PopID();
	}

	void AssetListWidget::drawObjectItem(/*std::shared_ptr<ResourceObject> obj*/)  
	{
		////ImGui::PushID(obj->getAsset().name.c_str());
		//
		//// 开始垂直布局
		//ImGui::BeginGroup();
		//
		//// 1. 绘制按钮区域
		//ImVec4 objColor = ImVec4(0.9f, 0.8f, 0.3f, 1.0f);
		//ImGui::PushStyleColor(ImGuiCol_Button, objColor);
		//bool clicked = ImGui::Button("##ObjectThumbnail", ImVec2(m_thumbnailSize, m_thumbnailSize));
		//ImGui::PopStyleColor();
		//
		//// 2. 在按钮右上角添加未保存标记
		//ImVec2 buttonMin = ImGui::GetItemRectMin();
		//ImGui::SetCursorScreenPos(ImVec2(
		//	buttonMin.x + m_thumbnailSize - ImGui::GetFontSize() - 2, // 右侧留2px边距
		//	buttonMin.y + 2
		//));
		//ImGui::TextColored(ImVec4(1, 0.2f, 0.2f, 1), "*");
		//
		//// 3. 在按钮下方绘制对象名称
		//std::string displayName = obj->getAsset().name.length() > 15 ?
		//	obj->getAsset().name.substr(0, 12) + "..." : obj->getAsset().name;
		//ImVec2 textSize = ImGui::CalcTextSize(displayName.c_str());
		//
		//// 计算名称位置（按钮下方居中）
		//ImGui::SetCursorScreenPos(ImVec2(
		//	buttonMin.x + (m_thumbnailSize - textSize.x) * 0.5f,
		//	buttonMin.y + m_thumbnailSize + ImGui::GetStyle().ItemSpacing.y
		//));
		//ImGui::Text("%s", displayName.c_str());
		//
		//// 4. 在名称下方添加未保存状态提示
		////if (!obj.isSaved) {
		//ImGui::SetCursorScreenPos(ImVec2(
		//	buttonMin.x + (m_thumbnailSize - ImGui::CalcTextSize("(unsaved)").x) * 0.5f,
		//	buttonMin.y + m_thumbnailSize + ImGui::GetTextLineHeight() + ImGui::GetStyle().ItemSpacing.y * 2
		//));
		//ImGui::TextColored(ImVec4(1, 0.5f, 0, 1), "(unsaved)");
		////}
		//
		//ImGui::EndGroup();
		//
		//// 交互处理
		////if (clicked) handleObjectClick(obj);
		////if (ImGui::IsItemHovered()) showObjectTooltip(obj);
		//
		//ImGui::PopID();

	}

	//void AssetListWidget::handleObjectClick(std::shared_ptr<ResourceObject> obj)
	//{
	//	// 清除所有对象的选中状态
	//	for (auto& item : m_assetObjects) {
	//		item->getPackage().pkIsSelect = false;
	//	}


	//	//设置当前对象为选中状态
	//	obj->getPackage().pkIsSelect = true;

	//}

}
