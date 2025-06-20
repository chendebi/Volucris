#ifndef __volucris_rhi_frame_buffer_h__
#define __volucris_rhi_frame_buffer_h__

#include <Engine/RHI/RHIResource.h>

namespace volucris
{
	class RHIRenderTarget : public RHIResource
	{
	public:
		RHIRenderTarget();

	protected:
		uint32 create() override;

		void bind(RHIState* state) override;

		void destroy(RHIState* state) override;

	private:

	};
}

#endif // !__volucris_rhi_frame_buffer_h__
