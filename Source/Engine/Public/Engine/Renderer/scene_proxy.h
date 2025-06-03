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

	struct DirectionLight
	{
		glm::vec4 color;
		glm::vec4 direction;
		float intensity;
	};

	class SceneProxy
	{
	public:
		SceneProxy();

		~SceneProxy();

		void initialize(Scene* scene);

		void render(Context* context);

		void addViews(const std::vector<std::shared_ptr<ViewProxy>>& views);

		void addPrimitiveProxy(const std::shared_ptr<PrimitiveProxy>& proxy);

		void removePrimitiveProxy(PrimitiveProxy* proxy);

		void addRenderPass(const std::shared_ptr<RenderPass>& pass);

		void setDirectionLightData(const DirectionLight& light);

	private:
		std::vector<std::shared_ptr<ViewProxy>> m_views;
		std::vector<std::shared_ptr<PrimitiveProxy>> m_primitives;
		std::shared_ptr<OGLBufferObject> m_ubo; // 存储场景数据
		UniformBlock m_directionLightBlock;
	};
}

#endif // !__volucris_scene_proxy_h__
