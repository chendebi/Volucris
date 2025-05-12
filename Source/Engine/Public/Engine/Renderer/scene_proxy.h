#ifndef __volucris_scene_proxy_h__
#define __volucris_scene_proxy_h__

#include <memory>
#include <vector>
#include "Engine/Renderer/proxy_object.h"

namespace volucris
{
	class Scene;
	class PrimitiveProxy;
	class ViewportProxy;

	class SceneProxy : public ProxyObject
	{
	public:
		SceneProxy(Scene* scene);

		~SceneProxy();

		void update();

		void render();

		void markSceneObjectRemoved();

		void addViewportProxy(const std::shared_ptr<ViewportProxy>& view);

		void addPrimitiveProxy(const std::shared_ptr<PrimitiveProxy>& proxy);

	private:
		std::vector<std::shared_ptr<ViewportProxy>> m_views;
		std::vector<std::shared_ptr<PrimitiveProxy>> m_primitives;
	};
}

#endif // !__volucris_scene_proxy_h__
