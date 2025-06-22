#include <RHI/RHISamplerState.h>
#include <glad/glad.h>
#include <RHI/RHIState.h>

namespace volucris
{
	RHISamplerState::RHISamplerState()
		: RHIResource()
	{

	}

	bool RHISamplerState::init(RHICommandList* command)
	{
		glBindSampler()
	}

	uint32 RHISamplerState::create(RHICommandList* command)
	{
		uint32 id;
		glGenSamplers(1, &id);
		return id;
	}

	void RHISamplerState::bind(RHIState* state)
	{
		if (state->texture2d)
		{
			glBindSampler(0, getId());
		}
	}

	void RHISamplerState::destroy(RHIState* state)
	{
		auto id = getId();
		glDeleteSamplers(1, &id);
	}
}