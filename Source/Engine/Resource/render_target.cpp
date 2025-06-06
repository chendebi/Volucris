#include "Resource/render_target.h"
#include <Renderer/render_target_proxy.h>
#include <Application/application.h>
#include <Renderer/renderer.h>

namespace volucris
{
	RenderTarget::RenderTarget(bool bClient)
		: m_dirty(true)
		, m_isClient(bClient)
		, m_rect({0,0,0,0})
		, m_proxy()
	{
	}

	std::shared_ptr<RenderTargetProxy> RenderTarget::getProxy()
	{
		std::shared_ptr<RenderTargetProxy> proxy = m_proxy.lock();
		if (!proxy)
		{
			proxy = std::make_shared<RenderTargetProxy>();
			proxy->initialize(this);
			m_proxy = proxy;
		}
		return proxy;
	}

	void RenderTarget::update() 
	{
		if (!m_dirty) return;
		if (!m_proxy.expired())
		{
			if (auto proxy = m_proxy.lock())
			{
				gApp->getRenderer()->pushCommand([proxy, rect = m_rect]() {
					proxy->setRect(rect);
					});
			}
		}
		m_dirty = false;
	}
}
