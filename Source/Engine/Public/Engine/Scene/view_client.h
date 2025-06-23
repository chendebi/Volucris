#ifndef __volucris_view_client_h__
#define __volucris_view_client_h__

#include "Engine/Core/rect.h"
#include "Engine/Core/size.h"
#include "Engine/Core/types_help.h"
#include <memory>
#include "Engine/Scene/scene_object.h"
#include <Engine/Core/event.h>

namespace volucris
{
	class ViewProxy;
	class RenderTarget;
	class PlayerActor;
	class EventHandler;

	class ViewClient : public SceneObject
	{
	public:
		enum class EventScope {
			ClientOnly, // 只接受当前视口的事件
			WholeWindow      // 接收整个窗口事件
		};

	public:
		ViewClient(const std::shared_ptr<RenderTarget>& target);

		void setClientRect(const Rect& rect);

		void update();

		bool isRenderProxyCreated() const { return !m_proxy.expired(); }

		std::shared_ptr<ViewProxy> getProxy();

		void setTargetGLTextureID(uint32 id);

		uint32 getTargetGLTextureID() const { return m_targetGLTextureID; }

		std::shared_ptr<RenderTarget> getRenderTarget() const { return m_target; }

		void setPlayer(const std::shared_ptr<PlayerActor>& player);

		PlayerActor* getPlayerActor() const { return m_player.get(); }

		void addEventHandler(const std::shared_ptr<EventHandler>& handler)
		{
			m_handlers.push_back(handler);
		}

		void removeEventHandler(EventHandler* handler);

		void setEventScope(EventScope scope) { m_scope = scope; }

		EventScope getEventScope() const { return m_scope; }

		bool isHardwareClient() const;

		void dispatchEvent(ClientEvent* event);

		Rect getRect() const { return m_rect; }

	private:
		friend class Scene;
		friend class SceneProxy;
		EventScope m_scope;
		Rect m_rect;
		std::weak_ptr<ViewProxy> m_proxy;
		uint32 m_targetGLTextureID;
		std::shared_ptr<RenderTarget> m_target;
		std::shared_ptr<PlayerActor> m_player;
		std::vector<std::shared_ptr<EventHandler>> m_handlers;
	};
}

#endif // !__volucris_view_client_h__
