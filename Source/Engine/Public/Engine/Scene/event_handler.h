#ifndef __volucris_event_handler_h__
#define __volucris_event_handler_h__

#include <Engine/Core/event.h>

namespace volucris
{
	class EventHandler
	{
	public:
		virtual ~EventHandler() = default;

		virtual void mousePressEvent(const MouseEvent& e) {  }

		virtual void mouseReleaseEvent(const MouseEvent& e) {  }

		virtual void mouseMoveEvent(int x, int y) {  }

		virtual void keyPressedEvent(Key key, Modifiers modifiers) {}

		virtual void keyReleasedEvent(Key key, Modifiers modifiers) {}
	};
}

#endif // !__volucris_event_handler_h__
