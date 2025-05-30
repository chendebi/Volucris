#include "Scene/view_client.h"
#include <Application/application.h>
#include <Renderer/renderer.h>
#include <glad/glad.h>
#include "Core/volucris.h"
#include <Renderer/viewport_proxy.h>
#include "Core/assert.h"
#include <Resource/render_target.h>
#include <Scene/scene.h>
#include <Renderer/scene_proxy.h>
#include <Scene/camera_component.h>

namespace volucris
{
	ViewClient::ViewClient(const std::shared_ptr<RenderTarget>& target)
		: m_proxy()
		, m_targetGLTextureID(0)
		, m_target(target)
		, m_camera(nullptr)
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
		if (m_camera && m_camera->isRenderTransformDirty())
		{
			const auto& viewMat = m_camera->getViewMatrix();
			const auto& projMat = m_camera->getProjectionMatrix();
			const auto& viewProjMat = m_camera->getViewProjectionMatrix();
			auto proxy = getProxy();
			gApp->getRenderer()->pushCommand([proxy, viewMat, projMat, viewProjMat] {
				proxy->setViewMatrix(viewMat);
				proxy->setProjectionMatrix(projMat);
				proxy->setProjectionViewMatrix(viewProjMat);
				});
		}
		m_target->update();
	}

	void ViewClient::setTargetGLTextureID(uint32 id)
	{
		m_targetGLTextureID = id;
	}
}
