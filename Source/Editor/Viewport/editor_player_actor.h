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

	private:
		bool m_mousePressed;
		Point2D m_lastPos;
	};
}

#endif // !__volucris_editor_player_actor_h__
