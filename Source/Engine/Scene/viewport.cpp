#include "Scene/viewport.h"
#include <Application/application.h>
#include <Renderer/renderer.h>
#include <glad/glad.h>
#include "Core/volucris.h"
#include <Renderer/viewport_proxy.h>
#include "Core/assert.h"

namespace volucris
{
	Viewport::Viewport()
		: m_dirty(0)
		, m_viewport()
		, m_proxy(nullptr)
	{

	}

	void Viewport::setViewport(const Rect& vp)
	{
		m_viewport = vp;
		m_dirty = true;
	}

	void Viewport::update()
	{
		if (m_dirty && m_proxy)
		{
			gApp->getRenderer()->pushCommand([this, proxy= m_proxy]() {
				proxy->setViewport(m_viewport);
				});
			m_dirty = false;
		}
	}
}
