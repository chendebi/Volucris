#include "Renderer/render_target_proxy.h"
#include <Resource/render_target.h>
#include <Renderer/OpenGL/ogl_frame_buffer_object.h>
#include <Renderer/OpenGL/ogl_texture2d_object.h>
#include <Renderer/OpenGL/ogl_render_buffer_object.h>

namespace volucris
{
	RenderTargetProxy::RenderTargetProxy()
		: m_isClient(false)
		, m_rect({0,0,0,0})
		, m_targetColorTexture(nullptr)
		, m_framebuffer(nullptr)
	{

	}

	void RenderTargetProxy::initialize(RenderTarget* target)
	{
		m_isClient = target->isClientTarget();
		
		m_framebuffer = std::make_shared<FrameBufferObject>();
		m_targetColorTexture = std::make_shared<Texture2DObject>();
		m_targetColorTexture->setFormat(GL_RGB);
		m_targetColorTexture->setType(GL_UNSIGNED_BYTE);

		auto depthRbo = std::make_shared<RenderBufferObject>();
		depthRbo->setSize(m_rect.w, m_rect.h);
		depthRbo->setFormat(GL_DEPTH24_STENCIL8);
		m_framebuffer->attachDepth(depthRbo);

		m_framebuffer->attachColor(0, m_targetColorTexture);
		setRect(target->getRect());
	}

	void RenderTargetProxy::setRect(const Rect& rect)
	{
		m_rect = rect;
		m_framebuffer->resize(rect.w, rect.h);
	}

	bool RenderTargetProxy::isTargetDirty() const
	{
		return !m_targetColorTexture->isValid();
	}
}
