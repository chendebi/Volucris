#ifndef __volucris_editor_player_actor_h__
#define __volucris_editor_player_actor_h__

#include <Engine/Scene/player_actor.h>
#include <Engine/Core/rect.h>
#include <glm/glm.hpp>

namespace volucris
{
	class EditorPlayerActor : public PlayerActor
	{
	public:
		EditorPlayerActor();

		void mousePressEvent(MouseEvent* e, ViewClient* sender) override;

		void mouseReleaseEvent(MouseEvent* e, ViewClient* sender) override;

		void mouseMoveEvent(MouseMoveEvent* e, ViewClient* sender) override;

		void keyPressedEvent(KeyEvent* e, ViewClient* sender) override;

		void keyReleasedEvent(KeyEvent* e, ViewClient* sender) override;

		void tick(double delta) override;

	private:
		uint8 m_mousePressed;
		uint8 m_mousePosInited;
		glm::vec2 m_speed;
		Point2D m_lastPos;
	};
}

#endif // !__volucris_editor_player_actor_h__
