#include "Application/client_viewport.h"
#include <Application/application.h>
#include <Application/window.h>
#include "Core/volucris.h"

#define V_CALLBACK_FUNCTION_NAME(name) on##name##Changed
#define V_CALLBACK(name) &Impl::V_CALLBACK_FUNCTION_NAME(name)
#define V_DECLARE_SIZE_CHANGED_CALLBACK(name) void V_CALLBACK_FUNCTION_NAME(name)(int x, int y) { viewport->V_CALLBACK_FUNCTION_NAME(name)(x, y); }

namespace volucris
{
	struct ClientViewport::Impl
	{
		ClientViewport* viewport;
		V_DECLARE_SIZE_CHANGED_CALLBACK(FrameSize)
		V_DECLARE_SIZE_CHANGED_CALLBACK(MouseMove)
	};

	ClientViewport::ClientViewport()
		: Viewport()
		, m_impl(new Impl)
	{
		m_impl->viewport = this;
		auto window = gApp->getWindow();
		const auto size = window->getFrameSize();
		m_clientRect = { 0, 0, size.width, size.height };
		window->FrameSizeChanged.addObject(m_impl, V_CALLBACK(FrameSize));
		window->MouseMove.addObject(m_impl, V_CALLBACK(MouseMove));
	}

	ClientViewport::~ClientViewport()
	{
		auto window = gApp->getWindow();
		window->FrameSizeChanged.removeAll(m_impl);
		window->MouseMove.removeAll(m_impl);
		delete m_impl;
	}

	void ClientViewport::setClientRect(const Rect& rect)
	{
		m_clientRect = rect;
	}

	void ClientViewport::onFrameSizeChanged(int width, int height)
	{
		auto vp = Rect(0, 0, width, height);
		setClientRect(vp);
		auto window = gApp->getWindow();
		const auto size = window->getFrameSize();
		vp.y = size.height - vp.y - vp.h;
		V_LOG_DEBUG(Engine, "viewport size changed to: {}", vp.toString())
		setViewport(vp);
	}

	void ClientViewport::onMouseMoveChanged(int x, int y)
	{
		if (m_clientRect.contains(x, y))
		{
			MouseMove(x - m_clientRect.x, y - m_clientRect.y);
		}
	}
}
