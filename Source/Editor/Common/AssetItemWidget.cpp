#include "AssetItemWidget.h"
#include <imgui.h>
#include <imgui_internal.h>

namespace volucris
{
    ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs)
    {
        return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y);
	}

    ImVec2 operator-(const ImVec2& lhs, const ImVec2& rhs)
    {
        return ImVec2(lhs.x - rhs.x, lhs.y - rhs.y);
    }

	AssetItemWidget::AssetItemWidget(const std::string& name)
		: Widget()
        , m_name(name)
		, m_className()
		, m_assetData()
        , m_height(42)
	{

	}

	void AssetItemWidget::setClassName(const std::string& className)
	{
		m_className = className;
	}

	void AssetItemWidget::onBuild(bool)
	{
  //      ImGui::PushID(m_name.c_str()); // 确保控件唯一ID

  //      ImGui::Text("Asset");

		//ImGui::SameLine();
  //      ImGui::PushID("##Combo"); // 子控件独立ID
  //      int selected = 0;
  //      int* current_item = &selected;
  //      if (ImGui::BeginCombo("", m_className.c_str())) {
  //          for (int i = 0; i < 5; i++) {
  //              ImGui::PushID(i);
  //              bool is_selected = (*current_item == i);
  //              if (ImGui::Selectable("dasdas", is_selected)) {
  //                  *current_item = i;
  //              }
  //              if (is_selected) {
  //                  ImGui::SetItemDefaultFocus();
  //              }
  //              ImGui::PopID();
  //          }
  //          ImGui::EndCombo();
  //      }
  //      ImGui::PopID(); // 结束下拉框ID
  //      ImGui::PopID(); // 结束下拉框ID

		// 获取当前行高度
		float line_height = ImGui::GetFrameHeight();

		// 计算垂直居中所需的偏移量
		float text_offset_y = (line_height - ImGui::GetTextLineHeight()) * 0.5f;

		// 开始同一行布局
		ImGui::BeginGroup();

		// 绘制文本（添加垂直偏移实现中心对齐）
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + text_offset_y);
		ImGui::Text("选择选项:");

		// 保持在同一行
		ImGui::SameLine();

		// 绘制 Combo 控件
		//ImGui::SetNextItemWidth(150.0f); // 设置 Combo 宽度
		if (ImGui::BeginCombo("##options", "请选择")) {
			if (ImGui::Selectable("选项一")) {}
			if (ImGui::Selectable("选项二")) {}
			ImGui::EndCombo();
		}

		// 结束组布局
		ImGui::EndGroup();
	}


}

