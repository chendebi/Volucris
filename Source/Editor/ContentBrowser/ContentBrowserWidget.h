#ifndef __volucris_content_browser_widget_h__
#define __volucris_content_browser_widget_h__

#include <Engine/Application/Widget.h>

namespace volucris
{
	class ContentWidget;

	class ContentBrowserWidget : public Widget
	{
	public:
		ContentBrowserWidget();

	private:
		void onBuild(bool init) override;

	private:
		std::shared_ptr<ContentWidget> m_contentWidget;
	};
}

#endif // !__volucris_content_browser_widget_h__
