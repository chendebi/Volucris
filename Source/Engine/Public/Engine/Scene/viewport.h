#ifndef __volucris_viewport_h__
#define __volucris_viewport_h__

#include "Engine/Core/rect.h"
#include "Engine/Core/size.h"
#include "Engine/Core/types_help.h"
#include <memory>
#include "Engine/Scene/scene_object.h"
#include <Engine/Core/event.h>

DECLARE_EVENT(OnTargetGLTextureIDChanged, volucris::uint32)

namespace volucris
{
	class ViewProxy;
	class RenderTarget;

	class ViewClient : public SceneObject
	{
	public:
		OnTargetGLTextureIDChanged TargetGLTextureIDChanged;

	public:
		ViewClient(const std::shared_ptr<RenderTarget>& target);

		void setClientRect(const Rect& rect);

		void update();

		std::shared_ptr<ViewProxy> getProxy();

		void setTargetGLTextureID(uint32 id);

		std::shared_ptr<RenderTarget> getRenderTarget() const { return m_target; }

	private:
		friend class Scene;
		friend class SceneProxy;
		std::weak_ptr<ViewProxy> m_proxy;
		uint32 m_targetGLTextureID;
		std::shared_ptr<RenderTarget> m_target;
	};
}

#endif // !__volucris_viewport_h__
