#ifndef __volucris_viewport_h__
#define __volucris_viewport_h__

#include "Engine/Core/rect.h"
#include "Engine/Core/size.h"
#include "Engine/Core/types_help.h"
#include <memory>
#include "Engine/Scene/scene_object.h"

namespace volucris
{
	class ViewportProxy;

	class Viewport : public SceneObject
	{
	public:
		Viewport();

		void setViewport(const Rect& vp);

		Rect getViewport() const { return m_viewport; }

		void update();

		ViewportProxy* getProxy() const { return m_proxy; }

		const Rect& getViewportRect() const { return m_viewport; }

	private:
		friend class Scene;
		friend class SceneProxy;
		uint8 m_dirty;
		Rect m_viewport;
		ViewportProxy* m_proxy;
	};
}

#endif // !__volucris_viewport_h__
