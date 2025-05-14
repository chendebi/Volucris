#ifndef __volucris_scene_proxy_h__
#define __volucris_scene_proxy_h__

#include <memory>
#include <vector>

namespace volucris
{
	class Scene;
	class PrimitiveProxy;
	class ViewportProxy;
	class RenderPass;
	class Context;

	class SceneProxy
	{
	public:
		SceneProxy(Scene* scene);

		~SceneProxy();

		void render(Context* context);

		void addViewportProxy(const std::shared_ptr<ViewportProxy>& view);

		void addPrimitiveProxy(const std::shared_ptr<PrimitiveProxy>& proxy);

		void removePrimitiveProxy(PrimitiveProxy* proxy);

		void addRenderPass(const std::shared_ptr<RenderPass>& pass);

	private:
		std::vector<std::shared_ptr<ViewportProxy>> m_views;
		std::vector<std::shared_ptr<PrimitiveProxy>> m_primitives;
	};
}

#endif // !__volucris_scene_proxy_h__
