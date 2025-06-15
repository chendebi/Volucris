#ifndef __volucris_widget_h__
#define __volucris_widget_h__

#include <Engine/Core/Object.h>
#include <vector>

namespace volucris
{
	class Widget : public Object
	{
	public:
		Widget();

		~Widget();

		void addChild(std::shared_ptr<Widget> child);

		void setParent(Widget* parent);

		void build();

	protected:
		virtual void onBuild() {}

	private:
		Widget* m_parent;
		std::vector < std::shared_ptr<Widget>> m_children;
	};
}

#endif // !__volucris_widget_h__
