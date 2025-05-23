#ifndef __volicris_viewport_widget_h__
#define __volicris_viewport_widget_h__

#include <Engine/Application/widget.h>
#include <Engine/Scene/viewport.h>

namespace volucris
{
	class ViewportWidget : public Widget
	{
	public:
		ViewportWidget();

		void setScene(const std::shared_ptr<Scene>& scene)
		{
			m_scene = scene;
		}

		void build() override;

	private:
		std::shared_ptr<Scene> m_scene;
		std::shared_ptr<Viewport> m_viewport;
	};
}

#endif // !__volicris_viewport_widget_h__
