#include "Scene/viewport.h"
#include <Application/application.h>
#include <Renderer/renderer.h>
#include <glad/glad.h>
#include "Core/volucris.h"
#include <Renderer/viewport_proxy.h>
#include "Core/assert.h"
#include <Resource/render_target.h>

namespace volucris
{
	ViewClient::ViewClient(const std::shared_ptr<RenderTarget>& target)
		: m_proxy()
		, m_targetGLTextureID(0)
		, m_target(target)
	{

	}

	void ViewClient::setClientRect(const Rect& rect)
	{
		if (m_target)
		{
			m_target->setRect(rect);
		}
	}

	std::shared_ptr<ViewProxy> ViewClient::getProxy()
	{
		auto proxy = m_proxy.lock();
		if (!proxy)
		{
			proxy = std::make_shared<ViewProxy>();
			proxy->initialize(this);
			m_proxy = proxy;
		}
		return proxy;
	}

	void ViewClient::update()
	{
		m_target->update();
	}

	void ViewClient::setTargetGLTextureID(uint32 id)
	{
		m_targetGLTextureID = id;
		TargetGLTextureIDChanged.broadcast(id);
	}
}
