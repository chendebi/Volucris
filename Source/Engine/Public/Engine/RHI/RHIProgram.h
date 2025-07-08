#ifndef __volucris_rhi_program_h__
#define __volucris_rhi_program_h__

#include <Engine/RHI/RHIResource.h>

namespace volucris
{
	class RHIShader;
	class RHIProgram : public RHIResource
	{
	public:
		RHIProgram() : RHIResource() {}

		bool init(RHICommandList* command) override { return false; }

		bool init(RHICommandList* command, const std::vector<std::shared_ptr<RHIShader>>& shaders);

	protected:
		uint32 create(RHIState* state) override;

		void bind(RHIState* state) override;

		void destroy(RHIState* state) override;

	private:

	};
}

#endif // !__volucris_rhi_program_h__
