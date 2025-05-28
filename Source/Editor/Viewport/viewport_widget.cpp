#include "viewport_widget.h"
#include <imgui.h>
#include "EditorEntry/editor_core.h"
#include <Engine/Application/application.h>
#include <Engine/Scene/scene.h>
#include <Engine/Scene/camera_component.h>
#include <Engine/Scene/actor.h>

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
		ImVec2 size = ImGui::GetContentRegionAvail();
		const Rect& rect = m_viewport->getViewportRect();
		if ((int)size.x != rect.w || (int)size.y != rect.h)
		{
			m_viewport->setViewport({ 0, 0, (int)size.x, (int)size.y });
		}

		ImTextureID texID = (ImTextureID)(intptr_t)m_targetID;

		ImGui::Image(texID, size, ImVec2(0, 1), ImVec2(1, 0));

		ImGui::End();
		ImGui::PopStyleVar();
	}

	void ViewportWidget::setViewportTargetGLID(uint32 id)
	{
		m_targetID = id;
	}
}
