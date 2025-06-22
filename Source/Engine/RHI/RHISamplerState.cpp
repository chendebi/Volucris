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
		//command
	}

	uint32 RHISamplerState::create(RHICommandList* command)
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