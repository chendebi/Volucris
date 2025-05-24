#ifndef __volucris_viewport_proxy_h__
#define __volucris_viewport_proxy_h__

#include <Engine/Core/rect.h>
#include <vector>
#include "Engine/Renderer/mesh_render_data.h"
#include <memory>
#include <glm/glm.hpp>
#include "OpenGL/ogl_render_state.h"

namespace volucris
{
	class Viewport;
	class SceneProxy;
	class PrimitiveProxy;
	class MaterialProxy;
	class RenderPass;
	class Context;

	struct CameraInfo
	{
		glm::mat4 viewMatrix;
		glm::mat4 projMatrix;
		glm::mat4 projViewMatrix;
	};

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
			m_cameraInfo.viewMatrix = mat;
			m_cameraInfoDirty = true;
		}

		void setProjectionMatrix(const glm::mat4& mat)
		{
			m_cameraInfo.projMatrix = mat;
			m_cameraInfoDirty = true;
		}

		void setProjectionViewMatrix(const glm::mat4& mat)
		{
			m_cameraInfo.projViewMatrix = mat;
			m_cameraInfoDirty = true;
		}

	private:
		Rect m_viewport;
		Viewport* m_vp;
		SceneProxy* m_scene;
		std::vector<std::shared_ptr<RenderPass>> m_passes;
		CameraInfo m_cameraInfo;
		uint8 m_cameraInfoDirty;
		UniformBlock m_cameraInfoBlock;
		std::weak_ptr<Texture2DObject> m_target;
	};
}

#endif // !__volucris_viewport_proxy_h__
