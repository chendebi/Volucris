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
#include <Scene/event_handler.h>
#include <Scene/player_actor.h>
#include <Core/vector_help.h>

namespace volucris
{
	ViewClient::ViewClient(const std::shared_ptr<RenderTarget>& target)
		: m_rect()
		, m_proxy()
		, m_targetGLTextureID(0)
		, m_target(target)
		, m_player(nullptr)
		, m_handlers()
	{

	}

	void ViewClient::setClientRect(const Rect& rect)
	{
		m_rect = rect;
		if (m_target)
		{
			auto targetRect = rect;
			targetRect.w = std::min(rect.w, 1920);
			targetRect.h = targetRect.w * 1.0 * rect.h / rect.w;
			m_target->setRect(targetRect);
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
		if (!m_player)
		{
			return;
		}
		auto camera = m_player->getCameraComponent();

		if (camera && camera->isRenderTransformDirty())
		{
			const auto& viewMat = camera->getViewMatrix();
			const auto& projMat = camera->getProjectionMatrix();
			const auto& viewProjMat = camera->getViewProjectionMatrix();
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

	void ViewClient::setPlayer(const std::shared_ptr<PlayerActor>& player)
	{
		if (!player)
		{
			return;
		}

		if (auto scene = player->getScene())
		{
			if (scene != getScene())
			{
				V_LOG_WARN(Engine, "player has registered in other scene");
				return;
			}
		}
		else if (getScene())
		{
			getScene()->addActor(player);
		}
		
		if (m_player)
		{
			removeEventHandler(m_player.get());
		}
		m_player = player;
		addEventHandler(m_player);
	}

	void ViewClient::removeEventHandler(EventHandler* handler)
	{
		VectorHelp::quickRemoveFirstIf<std::shared_ptr<EventHandler>>(m_handlers, [handler](const std::shared_ptr<EventHandler>& exist)->bool {
			return exist.get() == handler;
			});
	}

	bool ViewClient::isHardwareClient() const
	{
		return m_target->isClientTarget();
	}

	void ViewClient::dispatchMousePressEvent(const MouseEvent& event)
	{
		for (const auto& handler : m_handlers)
		{
			handler->mousePressEvent(event);
		}
	}

	void ViewClient::dispatchMouseReleaseEvent(const MouseEvent& event)
	{
		for (const auto& handler : m_handlers)
		{
			handler->mouseReleaseEvent(event);
		}
	}

	void ViewClient::dispatchMouseMoveEvent(int x, int y)
	{
		if (!m_rect.contains(x, y))
		{
			return;
		}
		auto pos = m_rect.getRelativePoint(x, y);
		for (const auto& handler : m_handlers)
		{
			handler->mouseMoveEvent(pos.x, pos.y);
		}
	}

	void ViewClient::dispatchKeyPressedEvent(Key key, Modifiers modifiers)
	{
		for (const auto& handler : m_handlers)
		{
			handler->keyPressedEvent(key, modifiers);
		}
	}

	void ViewClient::dispatchKeyReleasedEvent(Key key, Modifiers modifiers)
	{
		for (const auto& handler : m_handlers)
		{
			handler->keyReleasedEvent(key, modifiers);
		}
	}
}
