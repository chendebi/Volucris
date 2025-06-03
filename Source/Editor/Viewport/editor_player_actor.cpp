#include "editor_player_actor.h"
#include <Engine/Scene/camera_component.h>
#include <EditorEntry/editor_core.h>
#include <Engine/Application/application.h>
#include <Engine/Application/window.h>
#include <Engine/Scene/view_client.h>

namespace volucris
{
	EditorPlayerActor::EditorPlayerActor()
		: PlayerActor()
		, m_mousePressed(false)
		, m_speed(0.0)
		, m_lastPos()
	{
		getCameraComponent()->setFOV(45.0);
		getCameraComponent()->setPosition({ 0, 3, 5 });
	}

	void EditorPlayerActor::mousePressEvent(MouseEvent* e, ViewClient* sender)
	{
		if (e->button == MouseEvent::RIGHT)
		{
			gApp->getWindow()->setCursorEnabled(false);
			sender->setEventScope(ViewClient::EventScope::WholeWindow);
			m_mousePressed = true;
			m_lastPos = Point2D();
		}
	}

	void EditorPlayerActor::mouseReleaseEvent(MouseEvent* e, ViewClient* sender)
	{
		if (e->button == MouseEvent::RIGHT)
		{
			gApp->getWindow()->setCursorEnabled(true);
			sender->setEventScope(ViewClient::EventScope::ClientOnly);
			m_mousePressed = false;
		}
	}

	void EditorPlayerActor::mouseMoveEvent(MouseMoveEvent* e, ViewClient* sender)
	{
		if (m_mousePressed)
		{
			if (m_lastPos.isValid())
			{
				Point2D pos = e->position;
				glm::vec2 moveStep = { m_lastPos.x - pos.x, m_lastPos.y - pos.y};
				glm::vec2 rot = moveStep * glm::vec2(2.0 * gApp->getFrameTime());
				auto camera = getCameraComponent();
				auto rotation = camera->getRotation();
				rotation.x += rot.y;
				rotation.y += rot.x;
				camera->setRotation(rotation);
			}
			m_lastPos = e->position;
		}
	}

	void EditorPlayerActor::keyPressedEvent(KeyEvent* e, ViewClient* sender)
	{
		if (e->key == Key::KEY_W)
		{
			m_speed.x = 2.0f;
		}
		else if (e->key == Key::KEY_S)
		{
			m_speed.x = -2.0f;
		}
		else if (e->key == Key::KEY_A)
		{
			m_speed.y = -2.0f;
		}
		else if (e->key == Key::KEY_D)
		{
			m_speed.y = 2.0f;
		}
	}

	void EditorPlayerActor::keyReleasedEvent(KeyEvent* e, ViewClient* sender)
	{
		if (e->key == Key::KEY_W || e->key == Key::KEY_S)
		{
			m_speed.x = 0.0f;
		}
		else if (e->key == Key::KEY_A || e->key == Key::KEY_D)
		{
			m_speed.y = 0.0f;
		}
	}

	void EditorPlayerActor::tick(double delta)
	{
		auto camera = getCameraComponent();
		float speedz = delta * m_speed.x;
		float speedx = delta * m_speed.y;
		auto pos = camera->getPosition() + camera->getForwardDirection() * glm::vec3(speedz, speedz, speedz)
			+ camera->getRightDirection() * glm::vec3(speedx, speedx, speedx);
		camera->setPosition(pos);
	}
}
