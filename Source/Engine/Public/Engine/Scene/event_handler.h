#ifndef __volucris_event_handler_h__
#define __volucris_event_handler_h__

#include <Engine/Core/event.h>

namespace volucris
{
	class ViewClient;

	class EventHandler
	{
	public:
		virtual ~EventHandler() = default;

		void handleEvent(ClientEvent* event, ViewClient* sender);

	protected:
		virtual void mousePressEvent(MouseEvent* e, ViewClient* sender) {  }

		virtual void mouseReleaseEvent(MouseEvent* e, ViewClient* sender) {  }

		virtual void mouseMoveEvent(MouseMoveEvent* e, ViewClient* sender) {  }

		virtual void keyPressedEvent(KeyEvent* e, ViewClient* sender) {}

		virtual void keyReleasedEvent(KeyEvent* e, ViewClient* sender) {}
	};
}

#endif // !__volucris_event_handler_h__
