#ifndef __volucris_widget_h__
#define __volucris_widget_h__

#include <Engine/Core/Object.h>
#include <Engine/Core/Size.h>
#include <Engine/Application/Event.h>
#include <vector>
#include <Engine/Core/Rect.h>

namespace volucris
{
	class Widget;

	class Widget : public Object
	{
	public:
		Widget();

		~Widget();

		void addChild(std::shared_ptr<Widget> child);

		void setParent(Widget* parent);

		void build();

		Widget* getTopWidget() 
		{
			if (m_parent)
			{
				return m_parent->getTopWidget();
			}
			return this;
		}
	protected:
		void topWidgetChanged(Widget* widget);

		virtual void onBuild() {}

		virtual void parentChanged(Widget* old, Widget* current) {}

		virtual void onTopWidgetChanged(Widget* widget) {}

	private:
		Widget* m_parent;
		std::vector <std::shared_ptr<Widget>> m_children;
		Rect m_rect;
	};
}

#endif // !__volucris_widget_h__
