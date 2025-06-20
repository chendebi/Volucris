#include <RHI/RHISamplerState.h>
#include <glad/glad.h>

namespace volucris
{
	RHISamplerState::RHISamplerState()
		: RHIResource()
	{

	}

	uint32 RHISamplerState::create()
	{
		uint32 id;
		glGenSamplers(1, &id);
		return id;
	}

	void RHISamplerState::destroy(RHIState* state)
	{
		auto id = getId();
		glDeleteSamplers(1, &id);
	}
}