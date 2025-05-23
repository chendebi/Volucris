#ifndef __volucris_main_widget_h__
#define __volucris_main_widget_h__

#include <Engine/Application/widget.h>

namespace volucris
{
	class ContentBrowserWidget;

	class MainWidget : public Widget
	{
	public:
		MainWidget();

		void build() override;

	private:
		float m_value;
		std::shared_ptr<ContentBrowserWidget> m_browserWidget;
	};
}

#endif // !__volucris_main_widget_h__
