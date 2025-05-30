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

		bool isHardwareClient() const;

		void dispatchMousePressEvent(const MouseEvent& event);
		
		void dispatchMouseReleaseEvent(const MouseEvent& event);

		void dispatchMouseMoveEvent(int x, int y);

	private:
		friend class Scene;
		friend class SceneProxy;
		std::weak_ptr<ViewProxy> m_proxy;
		uint32 m_targetGLTextureID;
		std::shared_ptr<RenderTarget> m_target;
		std::shared_ptr<PlayerActor> m_player;
		std::vector<std::shared_ptr<EventHandler>> m_handlers;
	};
}

#endif // !__volucris_view_client_h__
