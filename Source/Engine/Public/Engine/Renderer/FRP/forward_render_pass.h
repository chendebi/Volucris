#ifndef __volucris_forward_render_pass_h__
#define __volucris_forward_render_pass_h__

#include "Engine/Renderer/render_pass.h"
#include <Engine/Renderer/OpenGL/ogl_render_state.h>

namespace volucris
{
	class FrameBufferObject;

	class ForwardRenderPass : public RenderPass
	{
	public:
		ForwardRenderPass();

		void initialize(ViewProxy* viewport) override;

		void render(Context* context) override;

		void setPassTarget(const std::shared_ptr<FrameBufferObject>& target);

		std::shared_ptr<RenderPass> clone() {
			return std::make_shared<ForwardRenderPass>();
		}

		std::shared_ptr<Texture2DObject> getTargetTexture() const override;

	protected:
		bool shouldCollectBatch(const PrimitiveDrawBatch& batch) const { return true; }

	private:
		OGLClearState m_clearState;
		ViewProxy* m_view;
		std::shared_ptr<FrameBufferObject> m_target;
	};
}

#endif // !__volucris_forward_render_pass_h__
