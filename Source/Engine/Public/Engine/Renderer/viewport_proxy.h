#ifndef __volucris_viewport_proxy_h__
#define __volucris_viewport_proxy_h__

#include <Core/rect.h>
#include "Engine/Renderer/proxy_object.h"

namespace volucris
{
	class Viewport;
	class SceneProxy;

	class ViewportProxy : public ProxyObject
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
