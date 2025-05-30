#ifndef __volucris_render_target_proxy_h__
#define __volucris_render_target_proxy_h__

#include <Core/rect.h>
#include <memory>

namespace volucris
{
	class RenderTarget;
	class FrameBufferObject;
	class Texture2DObject;

	class RenderTargetProxy
	{
	public:
		RenderTargetProxy();

		void initialize(RenderTarget* target);

		Rect getViewport() const
		{
			return { 0, 0, m_rect.w, m_rect.h };
		}

		void setRect(const Rect& rect);

		Rect getRect() const { return m_rect; }

		bool isTargetDirty() const;

		std::shared_ptr<FrameBufferObject> getFrameBuffer() const
		{
			return m_framebuffer;
		}

		std::shared_ptr<Texture2DObject> getOutputColorTexture() const
		{
			return m_targetColorTexture;
		}

	private:
		bool m_isClient;
		Rect m_rect;
		std::shared_ptr<Texture2DObject> m_targetColorTexture;
		std::shared_ptr<FrameBufferObject> m_framebuffer;
	};
}

#endif // !__volucris_render_target_proxy_h__
