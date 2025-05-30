#include "editor_player_actor.h"
#include <Engine/Scene/camera_component.h>
#include <EditorEntry/editor_core.h>
#include <Engine/Application/application.h>

namespace volucris
{
	EditorPlayerActor::EditorPlayerActor()
		: PlayerActor()
		, m_mousePressed(false)
		, m_lastPos()
	{
		getCameraComponent()->setFOV(45.0);
		getCameraComponent()->setPosition({ 0, 3, 5 });
	}

	void EditorPlayerActor::mousePressEvent(const MouseEvent& e)
	{
		if (e.button == MouseButton::LEFT)
		{
			m_mousePressed = true;
			m_lastPos = Point2D();
		}
	}

	void EditorPlayerActor::mouseReleaseEvent(const MouseEvent& e)
	{
		if (e.button == MouseButton::LEFT)
		{
			m_mousePressed = false;
		}
	}

	void EditorPlayerActor::mouseMoveEvent(int x, int y)
	{
		if (m_mousePressed)
		{
			if (m_lastPos.isValid())
			{
				Point2D pos = { x, y };
				glm::vec2 moveStep = { x - m_lastPos.x, m_lastPos.y - y};
				glm::vec2 rot = moveStep * glm::vec2(2.0 * gApp->getFrameTime());
				auto camera = getCameraComponent();
				auto rotation = camera->getRotation();
				rotation.x += rot.x;
				rotation.y += rot.y;
				camera->setRotation(rotation);
			}
			m_lastPos = { x, y };
		}
	}
}
