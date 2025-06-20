#include <RHI/RHIRenderTarget.h>
#include <glad/glad.h>
#include <RHI/RHIState.h>

namespace volucris
{
	RHIRenderTarget::RHIRenderTarget()
		: RHIResource()
	{

	}

	uint32 RHIRenderTarget::create()
	{
		uint32 id;
		glGenFramebuffers(1, &id);
		return id;
	}

	void RHIRenderTarget::bind(RHIState* state)
	{
		if (state->frameBuffer == this)
		{
			return;
		}
		glBindFramebuffer(GL_FRAMEBUFFER, getId());
		state->frameBuffer = this;
	}
}