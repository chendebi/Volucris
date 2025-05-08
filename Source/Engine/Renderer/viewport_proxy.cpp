#include "Renderer/viewport_proxy.h"
#include "glad/glad.h"
#include "Core/volucris.h"
#include "Application/viewport.h"
#include "Scene/scene.h"

namespace volucris
{
	ViewportProxy::ViewportProxy(Viewport* viewport)
		: m_viewport()
		, m_scene(viewport->getScene()->getProxy())
	{

	}

	void ViewportProxy::setViewport(const Rect& vp)
	{
		m_viewport = vp;
		V_LOG_DEBUG(Engine, "viewport set to: {}", m_viewport.toString())
	}

	void ViewportProxy::render()
	{
		glViewport(m_viewport.x, m_viewport.y, m_viewport.w, m_viewport.h);
		glClearColor(0.2, 0.6, 0.8, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
	}
}
