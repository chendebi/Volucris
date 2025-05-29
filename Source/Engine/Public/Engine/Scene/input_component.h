#ifndef __volucris_input_component_h__
#define __volucris_input_component_h__

#include <Engine/Scene/component.h>
#include <Core/event.h>

namespace volucris
{
	class InputComponent : public Component
	{
	public:

	protected:
		friend class ViewClient;
		virtual void keyPressEvent(Key key) {}

		virtual void keyReleaseEvent(Key key) {}

		virtual void mousePressEvent(MouseEvent* event) { }
	};
}

#endif // !__volucris_input_component_h__
