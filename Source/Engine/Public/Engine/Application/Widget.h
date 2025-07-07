#ifndef __volucris_widget_h__
#define __volucris_widget_h__

#include <Engine/Core/Object.h>
#include <Engine/Core/Size.h>
#include <vector>
#include <Engine/Core/Rect.h>

namespace volucris
{
	class Widget;
	class RHICommandList;
	struct DropEvent;
	struct FocusEvent;

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

		RHICommandList* getContext() const { return m_context; }

	protected:
		void topWidgetChanged(Widget* old, Widget* current);

		bool setDropEvent(DropEvent* event);

		void setFocusEvent(FocusEvent* event);

		void rendererCreated(RHICommandList* cmdList);

		void beforeRedererDestroy(RHICommandList* cmdList);

		virtual void onBuild() {}

		virtual void parentChanged(Widget* old, Widget* current) {}

		virtual void onTopWidgetChanged(Widget* old, Widget* current) {}

		virtual void onRendererBuild(RHICommandList* cmdList) {}

		virtual void onRendererDestroy(RHICommandList* cmdList) {}

		virtual bool onDrop(DropEvent* event) { return false; }

		virtual void onWindowFocusChanged(FocusEvent* event) {}

	private:
		Widget* m_parent;
		std::vector <std::shared_ptr<Widget>> m_children;
		Rect m_rect;
		RHICommandList* m_context;
	};
}

#endif // !__volucris_widget_h__
