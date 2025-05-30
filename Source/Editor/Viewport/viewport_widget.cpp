#include "viewport_widget.h"
#include <imgui.h>
#include "EditorEntry/editor_core.h"
#include <Engine/Application/application.h>
#include <Engine/Scene/scene.h>
#include <Engine/Scene/camera_component.h>
#include <Engine/Scene/actor.h>
#include <Engine/Resource/resource_registry.h>
#include <Engine/Resource/static_mesh.h>
#include <Engine/Scene/primitive_component.h>
#include <Engine/Scene/level.h>
#include <Engine/Resource/render_target.h>

namespace volucris
{
	ViewportWidget::ViewportWidget()
		: Widget()
		, m_scene(nullptr)
		, m_targetID(0)
	{
		gApp->CurrentLevelChanged.addObject(this, &ViewportWidget::setLevel);
		setLevel(gApp->getCurrentLevel());
	}

	void ViewportWidget::build()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::Begin("viewport");

		if (ImGui::IsWindowFocused())
		{
			// 允许鼠标操作
		}

		ImVec2 size = ImGui::GetContentRegionAvail();
		
		if (m_client)
		{
			const Rect& rect = m_client->getRenderTarget()->getRect();
			if ((int)size.x != rect.w || (int)size.y != rect.h)
			{
				m_client->setClientRect({ 0, 0, (int)size.x, (int)size.y });
			}
		}

		ImTextureID texID = (ImTextureID)(intptr_t)m_client->getTargetGLTextureID();;
		
		ImGui::Image(texID, size, ImVec2(0, 1), ImVec2(1, 0));

		if (true)
		{
			auto pos = ImGui::GetWindowPos();
			auto statInfo = gApp->getStatInfo();
			// 设置窗口位置（右上角，偏移 10px）
			auto winSize = ImGui::GetWindowSize();
			ImVec2 window_pos = ImVec2(
				pos.x + size.x - 10.0f,  // X: 右侧对齐
				pos.y + winSize.y - size.y + 10.f                    // Y: 顶部对齐
			);
			ImVec2 window_pivot = ImVec2(1.0f, 0.0f); // 锚点在右上角

			// 设置窗口属性
			ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pivot);
			ImGui::SetNextWindowSize(ImVec2(100, 80)); // 可选：固定窗口大小

			// 定义窗口样式（无标题栏、无背景、无交互）
			ImGuiWindowFlags flags =
				ImGuiWindowFlags_NoTitleBar |
				ImGuiWindowFlags_NoResize |
				ImGuiWindowFlags_NoMove |
				ImGuiWindowFlags_NoBackground |
				ImGuiWindowFlags_NoSavedSettings |
				ImGuiWindowFlags_NoFocusOnAppearing;

			//ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0)); // RGBA: A=0 透明
			// 开始绘制窗口
			if (ImGui::Begin("Status Overlay", nullptr, flags)) {
				// 竖排的两个文本
				ImGui::Text(fmt::format("fps: {:.4f}", statInfo.frameRate).c_str());
			}
			ImGui::End();
			//ImGui::PopStyleColor();
		}

		ImGui::End();
		ImGui::PopStyleVar();
	}

	void ViewportWidget::setViewportTargetGLID(uint32 id)
	{
		m_targetID = id;
	}

	void ViewportWidget::setLevel(const std::shared_ptr<Level>& level)
	{
		if (level)
		{
			m_client = level->addClient({ 0,0,128,128 });
		}
	}
}
