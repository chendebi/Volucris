#ifndef __volucris_rhi_sampler_state_h__
#define __volucris_rhi_sampler_state_h__

#include <Engine/RHI/RHIResource.h>

namespace volucris
{
	class RHISamplerState : public RHIResource
	{
	public:
		RHISamplerState();

	protected:
		uint32 create() override;

		void destroy(RHIState* state) override;
	};
}

#endif // !__volucris_rhi_sampler_state_h__
