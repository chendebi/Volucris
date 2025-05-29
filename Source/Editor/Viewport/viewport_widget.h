#ifndef __volicris_viewport_widget_h__
#define __volicris_viewport_widget_h__

#include <Engine/Application/widget.h>
#include <Engine/Scene/viewport.h>

namespace volucris
{
	class Level;

	class ViewportWidget : public Widget
	{
	public:
		ViewportWidget();

		void build() override;

		void setViewportTargetGLID(uint32 id);

		void setLevel(const std::shared_ptr<Level>& level);

	private:
		std::shared_ptr<Scene> m_scene;
		std::shared_ptr<ViewClient> m_client;
		uint32 m_targetID;
	};
}

#endif // !__volicris_viewport_widget_h__
