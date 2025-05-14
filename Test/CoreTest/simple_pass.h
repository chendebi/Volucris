#ifndef __simple_pass_h__
#define __simple_pass_h__

#include <Engine/Renderer/render_pass.h>
#include <Engine/Renderer/OpenGL/ogl_render_state.h>

using namespace volucris;

class SimplePass : public RenderPass
{
public:
	SimplePass() = default;

	void initialize(ViewportProxy* viewport) override;

	void render(Context* context) override;

	std::shared_ptr<RenderPass> clone() {
		return std::make_shared<SimplePass>();
	}

protected:
	bool shouldCollectBatch(const PrimitiveDrawBatch& batch) const { return true; }

private:
	OGLClearState m_clearState;
	ViewportProxy* m_view;
};

#endif // !__simple_pass_h__
