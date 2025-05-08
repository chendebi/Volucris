#ifndef __volucris_scene_proxy_h__
#define __volucris_scene_proxy_h__

#include <memory>
#include <vector>

namespace volucris
{
	class Scene;
	class PrimitiveProxy;
	class ViewportProxy;

	class SceneProxy
	{
	public:
		SceneProxy(Scene* scene);

		~SceneProxy();

		void addViewportProxy(const std::shared_ptr<ViewportProxy>& view);

		void update();

		void addPrimitiveProxy(const std::shared_ptr<PrimitiveProxy>& proxy);

	private:
		Scene* m_scene;
		std::vector<std::shared_ptr<ViewportProxy>> m_views;
		std::vector<std::shared_ptr<PrimitiveProxy>> m_primitives;
	};
}

#endif // !__volucris_scene_proxy_h__
