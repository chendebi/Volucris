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

namespace volucris
{
	ViewportWidget::ViewportWidget()
		: Widget()
		, m_scene(nullptr)
		, m_viewport(std::make_shared<Viewport>())
		, m_targetID(0)
	{
		m_viewport->TargetGLTextureIDChanged.addObject(this, &ViewportWidget::setViewportTargetGLID);
	}

	void ViewportWidget::build()
	{
		if (!m_scene)
		{
			m_scene = std::make_shared<Scene>();
			m_scene->addViewport(m_viewport);
			auto camera = std::make_shared<CameraComponent>(CameraComponent::PERSPECTIVE, m_viewport.get());
			camera->setPosition({ 0, 2, 6 });
			auto actor = std::make_shared<Actor>();
			actor->setRootComponent(camera);
			m_scene->addActor(actor);

			gApp->addScene(m_scene);
		}

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::Begin("viewport");

		if (ImGui::IsWindowFocused())
		{
			// 允许
		}

		ImVec2 size = ImGui::GetContentRegionAvail();
		
		const Rect& rect = m_viewport->getViewportRect();
		if ((int)size.x != rect.w || (int)size.y != rect.h)
		{
			m_viewport->setViewport({ 0, 0, (int)size.x, (int)size.y });
		}

		ImTextureID texID = (ImTextureID)(intptr_t)m_targetID;
		
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
}
