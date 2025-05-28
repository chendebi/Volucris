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
		, m_targetGLTextureID(0)
	{

	}

	void Viewport::setViewport(const Rect& vp)
	{
		m_viewport = vp;
		ViewportSizeChanged.broadcast(vp.w, vp.h);
		m_dirty = true;
	}

	void Viewport::update()
	{
		if (m_dirty && m_proxy)
		{
			gApp->getRenderer()->pushCommand([vp = m_viewport, proxy= m_proxy]() {
				proxy->setViewport(vp);
				});
			m_dirty = false;
		}
	}

	void Viewport::setTargetGLTextureID(uint32 id)
	{
		m_targetGLTextureID = id;
		TargetGLTextureIDChanged.broadcast(id);
	}
}
