#ifndef __volucris_forward_render_pass_h__
#define __volucris_forward_render_pass_h__

#include "Engine/Renderer/render_pass.h"
#include <Engine/Renderer/OpenGL/ogl_render_state.h>

namespace volucris
{
	class ForwardRenderPass : public RenderPass
	{
	public:
		ForwardRenderPass();

		void initialize(ViewportProxy* viewport) override;

		void render(Context* context) override;

		std::shared_ptr<RenderPass> clone() {
			return std::make_shared<ForwardRenderPass>();
		}

	protected:
		bool shouldCollectBatch(const PrimitiveDrawBatch& batch) const { return true; }

	private:
		OGLClearState m_clearState;
		ViewportProxy* m_view;
	};
}

#endif // !__volucris_forward_render_pass_h__
