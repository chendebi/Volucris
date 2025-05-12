#ifndef __volucris_viewport_h__
#define __volucris_viewport_h__

#include "Engine/Core/rect.h"
#include "Engine/Core/types_help.h"
#include <memory>
#include "Engine/Scene/scene_object.h"
#include "Engine/Scene/renderable_object.h"

namespace volucris
{
	class ViewportProxy;

	class Viewport : public SceneObject, public RenderableObject
	{
	public:
		Viewport();

		void setViewport(const Rect& vp);

		Rect getViewport() const { return m_viewport; }

		void update();

	private:
		friend class Scene;
		friend class SceneProxy;
		uint8 m_dirty;
		Rect m_viewport;
	};
}

#endif // !__volucris_viewport_h__
