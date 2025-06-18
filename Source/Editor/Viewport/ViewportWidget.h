#ifndef __volucris_viewport_widget_h__
#define __volucris_viewport_widget_h__

#include <Engine/Application/Widget.h>

namespace volucris
{
	class ViewportWidget : public Widget
	{
	public:

	protected:
		void onBuild() override;
	};
}

#endif // !__volucris_viewport_widget_h__
