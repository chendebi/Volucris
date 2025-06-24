#ifndef __volucris_rhi_frame_buffer_h__
#define __volucris_rhi_frame_buffer_h__

#include <Engine/RHI/RHIResource.h>
#include "RHITexture.h"

namespace volucris
{

	class RHIRenderTarget : public RHIResource
	{
	public:
		RHIRenderTarget(const Size& size);

		void attachColor(RHITextureDesc desc, int32 index);
		
		/*void attachDepth(const RHITextureDesc& desc);

		void attachStencil(const RHITextureDesc& desc);

		void attachDepthStencil(const RHITextureDesc& desc);*/

		bool init(RHICommandList* command) override;

	protected:
		uint32 create(RHICommandList* command) override;

		void bind(RHIState* state) override;

		void destroy(RHICommandList* command) override;

	private:
		Size m_size;
		std::unordered_map<int32, std::shared_ptr<RHIResource>> m_colorAttachments;
		std::shared_ptr<RHIResource> m_depthAttachment;
	};
}

#endif // !__volucris_rhi_frame_buffer_h__
