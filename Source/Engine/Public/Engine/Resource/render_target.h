#ifndef __volucris_render_target_h__
#define __volucris_render_target_h__

#include <Engine/Core/rect.h>
#include <Engine/Resource/resource_object.h>
#include <Engine/Core/event.h>

DECLARE_EVENT(OnSizeChanged, int, int)

namespace volucris
{
	class RenderTargetProxy;

	class RenderTarget : public ResourceObject
	{
	public:
		OnSizeChanged SizeChanged;

		RenderTarget(bool bClient = false);

		virtual ~RenderTarget() {}

		void setPosition(int x, int y)
		{
			m_rect.x = x;
			m_rect.y = y;
			m_dirty = true;
		}

		void setRect(const Rect& rect)
		{
			if (onResized(rect.w, rect.h))
			{
				m_rect = rect;
				m_dirty = true;
				SizeChanged(rect.w, rect.h);
			}
		}

		Rect getRect() const
		{
			return m_rect;
		}

		Point2D getPosition() const
		{
			return { m_rect.x, m_rect.y };
		}

		bool isClientTarget() const { return m_isClient; }

		void resize(int w, int h)
		{
			if (onResized(w, h))
			{
				m_rect.w = w;
				m_rect.h = h;
				m_dirty = true;
				SizeChanged(w, h);
			}
		}

		std::shared_ptr<RenderTargetProxy> getProxy();

		void update();

	protected:
		virtual bool onResized(int w, int h) { return true; }

	protected:
		bool m_dirty;
		bool m_isClient;
		Rect m_rect;
		std::weak_ptr<RenderTargetProxy> m_proxy;
	};
}

#endif // !__volucris_render_target_h__
