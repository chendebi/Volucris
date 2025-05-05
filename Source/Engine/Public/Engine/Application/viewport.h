#ifndef __volucris_viewport_h__
#define __volucris_viewport_h__

#include "Engine/Application/widget.h"
#include "Engine/Core/rect.h"

namespace volucris
{
	class Viewport : public Widget
	{
	public:

	protected:
		void setViewport(const Rect& vp);

	private:
		Rect m_viewport;
	};
}

#endif // !__volucris_viewport_h__
