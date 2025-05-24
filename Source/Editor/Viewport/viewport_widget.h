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

		void setViewportTargetGLID(uint32 id);

	private:
		std::shared_ptr<Scene> m_scene;
		std::shared_ptr<Viewport> m_viewport;
		uint32 m_targetID;
	};
}

#endif // !__volicris_viewport_widget_h__
