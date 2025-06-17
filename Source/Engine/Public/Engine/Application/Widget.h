#ifndef __volucris_widget_h__
#define __volucris_widget_h__

#include <Engine/Core/Object.h>
#include <Engine/Core/Size.h>
#include <Engine/Application/Event.h>
#include <vector>

namespace volucris
{
	class Widget;

	DECLARE_EVENT(OnParentChanged, Widget*)

	class Widget : public Object
	{
	public:
		OnParentChanged ParentChanged;

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
		virtual void onBuild() {}

	private:
		Widget* m_parent;
		std::vector <std::shared_ptr<Widget>> m_children;
	};
}

#endif // !__volucris_widget_h__
