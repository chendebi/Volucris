#ifndef __volucris_viewport_widget_h__
#define __volucris_viewport_widget_h__

#include <Engine/Application/Widget.h>

namespace volucris
{
	class View;

	class ViewportWidget : public Widget
	{
	public:
		ViewportWidget();

	protected:
		void onBuild() override;

		void onTopWidgetChanged(Widget* widget) override;

		void viewSizeChanged(Size size);

	private:
		View* m_view;
		Size m_size;
	};
}

#endif // !__volucris_viewport_widget_h__
