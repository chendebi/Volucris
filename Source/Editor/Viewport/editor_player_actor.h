#ifndef __volucris_editor_player_actor_h__
#define __volucris_editor_player_actor_h__

#include <Engine/Scene/player_actor.h>
#include <Engine/Core/rect.h>

namespace volucris
{
	class EditorPlayerActor : public PlayerActor
	{
	public:
		EditorPlayerActor();

		void mousePressEvent(const MouseEvent& e) override;

		void mouseReleaseEvent(const MouseEvent& e) override;

		void mouseMoveEvent(int x, int y) override;

		void keyPressedEvent(Key key, Modifiers modifiers) override;

		void keyReleasedEvent(Key key, Modifiers modifiers) override;

		void tick(double delta) override;

	private:
		bool m_mousePressed;
		float m_speed;
		Point2D m_lastPos;
	};
}

#endif // !__volucris_editor_player_actor_h__
