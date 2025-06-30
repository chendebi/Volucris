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
		return false;
	}

	uint32 RHISamplerState::create(RHIState* state)
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