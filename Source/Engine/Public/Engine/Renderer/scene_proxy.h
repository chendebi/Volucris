#ifndef __volucris_scene_proxy_h__
#define __volucris_scene_proxy_h__

#include <memory>
#include <vector>
#include <Engine/Renderer/OpenGL/ogl_render_state.h>

namespace volucris
{
	class Scene;
	class PrimitiveProxy;
	class ViewProxy;
	class RenderPass;
	class Context;

	class SceneProxy
	{
	public:
		SceneProxy(Scene* scene);

		~SceneProxy();

		void render(Context* context);

		void addView(const std::shared_ptr<ViewProxy>& view);

		void addPrimitiveProxy(const std::shared_ptr<PrimitiveProxy>& proxy);

		void removePrimitiveProxy(PrimitiveProxy* proxy);

		void addRenderPass(const std::shared_ptr<RenderPass>& pass);

		UniformBlock addSceneData(uint8* data, size_t size);

		void setSceneData(const UniformBlock& block, uint8* data);

	private:
		std::vector<std::shared_ptr<ViewProxy>> m_views;
		std::vector<std::shared_ptr<PrimitiveProxy>> m_primitives;
		std::shared_ptr<OGLBufferObject> m_ubo; // 存储场景数据
	};
}

#endif // !__volucris_scene_proxy_h__
