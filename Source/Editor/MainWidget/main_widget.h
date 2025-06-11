#ifndef __volucris_main_widget_h__
#define __volucris_main_widget_h__

#include <Engine/Application/widget.h>
#include <Viewport/viewport_widget.h>

namespace volucris
{
	class LogWidget;
	class PropertyWidget;
	class ContentBrowserWidget;

	class MainWidget : public Widget
	{
	public:
		MainWidget();

		void build() override;

		void setLogWidget(const std::shared_ptr<LogWidget>& widget)
		{
			m_logWidget = widget;
		}

		void setPropertyWidget(const std::shared_ptr<PropertyWidget>& widget)
		{
			m_propertyWidget = widget;
		}

	private:
		float m_value;
		std::shared_ptr<ContentBrowserWidget> m_browserWidget;
		std::shared_ptr<LogWidget> m_logWidget;
		std::shared_ptr<PropertyWidget> m_propertyWidget;
		ViewportWidget m_viewport;
	};
}

#endif // !__volucris_main_widget_h__
