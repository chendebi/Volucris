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
	class CameraComponent;

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

		void bindCamera(const std::shared_ptr<CameraComponent>& camera)
		{
			m_camera = camera;
		}

	private:
		friend class Scene;
		friend class SceneProxy;
		std::weak_ptr<ViewProxy> m_proxy;
		uint32 m_targetGLTextureID;
		std::shared_ptr<RenderTarget> m_target;
		std::shared_ptr<CameraComponent> m_camera;
	};
}

#endif // !__volucris_view_client_h__
