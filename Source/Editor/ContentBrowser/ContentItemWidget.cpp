#include <ContentBrowser/ContentItemWidget.h>
#include <Engine/RHI/RHITexture.h>
#include <imgui_internal.h>

namespace volucris
{
	void DrawTextCenteredInRect(const ImVec2& rect_min, const ImVec2& rect_max, float fontSize, const char* text, ImVec4 color = ImVec4(1, 1, 1, 1))
	{
		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		ImFont* font = ImGui::GetFont();

		// 1. 计算文字尺寸
		ImVec2 text_size = font->CalcTextSizeA(fontSize, FLT_MAX, 0.0f, text);

		// 2. 计算矩形尺寸
		float rect_width = rect_max.x - rect_min.x;
		float rect_height = rect_max.y - rect_min.y;

		bool text_fits = (text_size.x <= rect_width);

		// 设置裁剪区域（防止文字溢出）
		draw_list->PushClipRect(rect_min, rect_max, true);

		// 计算垂直居中位置
		float y_pos = rect_min.y + (rect_height - text_size.y) * 0.5f;

		if (text_fits) {
			// 文字未超出 - 水平居中
			float x_pos = rect_min.x + (rect_width - text_size.x) * 0.5f;
			draw_list->AddText(font, fontSize, ImVec2(x_pos, y_pos), ImGui::ColorConvertFloat4ToU32(color), text);
		}
		else {
			// 文字超出 - 左对齐并截断
			draw_list->AddText(font, fontSize, ImVec2(rect_min.x, y_pos), ImGui::ColorConvertFloat4ToU32(color), text);
		}

		// 恢复裁剪区域
		draw_list->PopClipRect();

		// 3. 计算居中位置
		//ImVec2 text_pos = rect_min;
		//text_pos.x += (rect_width - text_size.x) * 0.5f; // 水平居中
		//text_pos.y += (rect_height - text_size.y) * 0.5f; // 垂直居中

		//// 4. 绘制文字
		//draw_list->AddText(font, fontSize, text_pos, ImGui::ColorConvertFloat4ToU32(color), text);

		//// 5. (可选)绘制矩形边框
		//draw_list->AddRect(rect_min, rect_max, IM_COL32(255, 0, 0, 128));
	}

	ContentItemWidget::ContentItemWidget()
		: m_texture(nullptr)
		, m_minUV()
		, m_maxUV()
		, m_size(96, 128)
		, m_iconSpace()
		, m_iconSize()
		, m_hoverColor({ 0.9411, 0.850, 0.559 ,1.0})
		, m_selectedColor({ 0.882, 0.725, 0.219 ,1.0 })
		, m_selected(false)
	{
		setScale(1.0);
	}

	ContentItemWidget::ContentItemWidget(RHITexture2D* texture, Point iconPos, Size iconSize)
		: ContentItemWidget()
	{
		setIcon(texture, iconPos, iconSize);
	}

	void ContentItemWidget::setIcon(RHITexture2D* texture, Point iconPos, Size iconSize)
	{
		auto texSize = texture->getSize();
		auto ustep = iconSize.width * 1.0f / texSize.width;
		auto vstep = iconSize.height * 1.0f / texSize.height;

		m_texture = texture;
		m_minUV = { iconPos.x * ustep, 1.0f - iconPos.y * vstep };
		m_maxUV = { m_minUV.x + ustep, m_minUV.y - vstep };
	}

	void ContentItemWidget::setScale(float scale)
	{
		m_size = m_size * scale;
		float iconSize = 88 * scale;
		m_iconSize = { iconSize , iconSize };
		m_iconSpace.x = 4 * scale;
		m_iconSpace.y = 4 * scale;

		m_fontSize = 16 * scale;
	}

	void ContentItemWidget::build()
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window->SkipItems)
			return;

		const ImVec2 size(m_size.x, m_size.y);
		ImVec2 cursorPos = ImGui::GetCursorScreenPos();

		ImGui::InvisibleButton("##xx", size);
		bool clicked = ImGui::IsItemClicked();
		bool hovered = ImGui::IsItemHovered();

		if (clicked)
		{
			m_selected = true;
		}

		if (hovered)
		{
			window->DrawList->AddRectFilled(
				cursorPos, { cursorPos.x + m_size.x, cursorPos.y + m_size.y },
				IM_COL32(82, 82, 82, 255)
			);
		}
		else if (m_selected)
		{
			window->DrawList->AddRectFilled(
				cursorPos, { cursorPos.x + m_size.x, cursorPos.y + m_size.y },
				IM_COL32(49,49,49, 255)
			);
		}

		auto id = m_texture->getId();
		ImTextureID texID = (ImTextureID)(intptr_t)id;
		ImVec2 imgPos = { cursorPos.x + m_iconSpace.x, cursorPos.y + m_iconSpace.y };

		window->DrawList->AddImage(texID,
			imgPos,
			ImVec2(imgPos.x + m_iconSize.x, imgPos.y + m_iconSize.y),
			{m_minUV.x, m_minUV.y},
			{m_maxUV.x, m_maxUV.y});

		ImVec2 fontRectMin = { cursorPos.x + m_iconSpace.x, imgPos.y + m_iconSize.y + m_iconSpace.y };
		ImVec2 fontRectMax = { cursorPos.x + m_size.x - m_iconSpace.x, cursorPos.y + m_size.y};
		DrawTextCenteredInRect(fontRectMin, fontRectMax, m_fontSize, "Test");
	}
}