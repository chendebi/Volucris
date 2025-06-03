#include "Scene/event_handler.h"

namespace volucris
{
	void EventHandler::handleEvent(ClientEvent* event, ViewClient* sender)
	{
		switch (event->type)
		{
		case ClientEvent::MOUSE_MOVE:
			mouseMoveEvent(static_cast<MouseMoveEvent*>(event), sender);
			break;
		case ClientEvent::MOUSE_BUTTON_PRESSED:
			mousePressEvent(static_cast<MouseEvent*>(event), sender);
			break;
		case ClientEvent::MOUSE_BUTTON_RELEASED:
			mouseReleaseEvent(static_cast<MouseEvent*>(event), sender);
			break;
		case ClientEvent::KEY_PRESSED:
			keyPressedEvent(static_cast<KeyEvent*>(event), sender);
			break;
		case ClientEvent::KEY_RELEASED:
			keyReleasedEvent(static_cast<KeyEvent*>(event), sender);
			break;
		default:
			break;
		}
	}
}
