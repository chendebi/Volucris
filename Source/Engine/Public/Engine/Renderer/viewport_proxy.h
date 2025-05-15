#ifndef __volucris_viewport_proxy_h__
#define __volucris_viewport_proxy_h__

#include <Engine/Core/rect.h>
#include <vector>
#include "Engine/Renderer/mesh_render_data.h"
#include <memory>
#include <glm/glm.hpp>

namespace volucris
{
	class Viewport;
	class SceneProxy;
	class PrimitiveProxy;
	class MaterialProxy;
	class RenderPass;
	class Context;

	class ViewportProxy
	{
	public:
		ViewportProxy(Viewport* viewport);

		void setViewport(const Rect& vp);

		void update(const std::vector<std::shared_ptr<PrimitiveProxy>>& primitives);

		void addRenderPass(const std::shared_ptr<RenderPass>& pass);

		const std::vector<std::shared_ptr<RenderPass>>& getPasses() const { return m_passes; }

		void render(Context* context);

		void clear();

		Rect getViewport() const { return m_viewport; }

		void setViewMatrix(const glm::mat4& mat)
		{
			m_viewMatrix = mat;
		}

		void setProjectionMatrix(const glm::mat4& mat)
		{
			m_projectionMatrix = mat;
		}

		void setProjectionViewMatrix(const glm::mat4& mat)
		{
			m_projectionViewMatrix = mat;
		}

	private:
		Rect m_viewport;
		SceneProxy* m_scene;
		std::vector<std::shared_ptr<RenderPass>> m_passes;
		glm::mat4 m_viewMatrix;
		glm::mat4 m_projectionMatrix;
		glm::mat4 m_projectionViewMatrix;
	};
}

#endif // !__volucris_viewport_proxy_h__
