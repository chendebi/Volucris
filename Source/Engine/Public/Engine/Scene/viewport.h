#ifndef __volucris_viewport_h__
#define __volucris_viewport_h__

#include "Engine/Core/rect.h"
#include "Engine/Core/size.h"
#include "Engine/Core/types_help.h"
#include <memory>
#include "Engine/Scene/scene_object.h"
#include <Engine/Core/event.h>

DECLARE_EVENT(OnTargetGLTextureIDChanged, volucris::uint32)
DECLARE_EVENT(OnViewportSizeChanged, int, int)

namespace volucris
{
	class ViewportProxy;

	class Viewport : public SceneObject
	{
	public:
		OnTargetGLTextureIDChanged TargetGLTextureIDChanged;
		OnViewportSizeChanged ViewportSizeChanged;

	public:
		Viewport();

		void setViewport(const Rect& vp);

		Rect getViewport() const { return m_viewport; }

		void update();

		ViewportProxy* getProxy() const { return m_proxy; }

		const Rect& getViewportRect() const { return m_viewport; }

		void setTargetGLTextureID(uint32 id);

	private:
		friend class Scene;
		friend class SceneProxy;
		uint8 m_dirty;
		Rect m_viewport;
		ViewportProxy* m_proxy;
		uint32 m_targetGLTextureID;
	};
}

#endif // !__volucris_viewport_h__
