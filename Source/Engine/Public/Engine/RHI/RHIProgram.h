#ifndef __volucris_rhi_program_h__
#define __volucris_rhi_program_h__

#include <Engine/RHI/RHIResource.h>

namespace volucris
{
	class RHIShader;
	class RHIProgram : public RHIResource
	{
	public:
		RHIProgram();

		~RHIProgram() override;

		uint32 getId();

		bool init(const std::vector<std::shared_ptr<RHIShader>>& shaders);

		bool isValid() const { return m_valid; }

	private:
		uint32 m_id;
		bool m_valid;
	};
}

#endif // !__volucris_rhi_program_h__
