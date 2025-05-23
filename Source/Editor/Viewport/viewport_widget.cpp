#include "viewport_widget.h"
#include <imgui.h>
#include "EditorEntry/editor_core.h"
#include <Engine/Application/application.h>
#include <Engine/Scene/scene.h>

namespace volucris
{
	ViewportWidget::ViewportWidget()
		: Widget()
		, m_scene(nullptr)
		, m_viewport(std::make_shared<Viewport>())
	{
	}

	void ViewportWidget::build()
	{
		if (!m_scene)
		{
			m_scene = std::make_shared<Scene>();
			m_scene->addViewport(m_viewport);
			gApp->addScene(m_scene);
		}

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::Begin("viewport");
		ImVec2 size = ImGui::GetContentRegionAvail();
		const Rect& rect = m_viewport->getViewportRect();
		if ((int)size.x != rect.w || (int)size.y != rect.h)
		{
			m_viewport->setViewport({ 0, 0, (int)size.x, (int)size.y });
		}
		ImGui::End();
		ImGui::PopStyleVar();
	}
}
