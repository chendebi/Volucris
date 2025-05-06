#ifndef __volucris_viewport_h__
#define __volucris_viewport_h__

#include "Engine/Core/rect.h"
#include "Engine/Core/types_help.h"
#include <memory>

namespace volucris
{
	class ViewportProxy;

	class Viewport
	{
	public:
		Viewport();

		void setViewport(const Rect& vp);

		Rect getViewport() const { return m_viewport; }

		void update();

		std::shared_ptr<ViewportProxy> createProxy();

		std::shared_ptr<ViewportProxy> getProxy() const { return m_proxy; }

	private:
		uint8 m_dirty;
		Rect m_viewport;
		std::shared_ptr<ViewportProxy> m_proxy;
	};
}

#endif // !__volucris_viewport_h__
