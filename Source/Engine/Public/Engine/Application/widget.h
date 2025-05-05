#ifndef __volucris_widget_h__
#define __volucris_widget_h__

#include <memory>
#include <vector>

namespace volucris
{
	class Widget
	{
	public:
		static void draw(Widget* widget);

		virtual void tick(double delta) {}

		virtual void build() = 0;

	private:
		std::vector<std::shared_ptr<Widget>> m_subwidgets;
	};
}

#endif // !__volucris_widget_h__
