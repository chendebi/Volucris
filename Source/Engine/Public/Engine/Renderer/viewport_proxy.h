#include <Core/rect.h>
#ifndef __volucris_viewport_proxy_h__
#define __volucris_viewport_proxy_h__

namespace volucris
{
	class Viewport;
	class SceneProxy;

	class ViewportProxy
	{
	public:
		ViewportProxy(Viewport* viewport);

		void setViewport(const Rect& vp);

		void render();

	private:
		Rect m_viewport;
		SceneProxy* m_scene;
	};
}

#endif // !__volucris_viewport_proxy_h__
