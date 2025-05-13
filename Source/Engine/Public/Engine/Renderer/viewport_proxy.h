#ifndef __volucris_viewport_proxy_h__
#define __volucris_viewport_proxy_h__

#include <Core/rect.h>
#include <vector>

namespace volucris
{
	class Viewport;
	class SceneProxy;
	class PrimitiveProxy;
	class MaterialProxy;

	struct RenderBatch
	{
		MaterialProxy* material;
		std::vector<SectionRenderData> sections;
		std::shared_ptr<MeshRenderData> renderData;
	};

	class ViewportProxy
	{
	public:
		ViewportProxy(Viewport* viewport);

		void setViewport(const Rect& vp);

		void update(const std::vector<std::shared_ptr<PrimitiveProxy>>& primitives);

		void render();

	private:
		Rect m_viewport;
		SceneProxy* m_scene;
		std::vector<RenderBatch> m_batches;
	};
}

#endif // !__volucris_viewport_proxy_h__
