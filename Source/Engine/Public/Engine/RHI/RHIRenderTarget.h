#ifndef __volucris_rhi_frame_buffer_h__
#define __volucris_rhi_frame_buffer_h__

#include <Engine/RHI/RHIResource.h>
#include <Engine/RHI/RHIState.h>
#include "RHITexture.h"

namespace volucris
{
	class RHIRenderTarget : public RHIResource
	{
	public:
		enum Usage
		{
			ReadOnly,
			WriteOnly,
			ReadWrite
		};

		RHIRenderTarget(const Size& size, Usage usage = ReadWrite);

		~RHIRenderTarget();

		void createGpuResource();

		void setUsage( RHICommandList* cmdList, Usage usage);

		void attachColor(const std::shared_ptr<RHITexture>& texture, int32 index);

		void attachDepth(const std::shared_ptr<RHITexture2D>& texture);
		
		RHIResource* getAttachedColor(int32 index) const
		{
			auto it = m_colorAttachments.find(index);
			if (it != m_colorAttachments.end())
			{
				return it->second.get();
			}
			return nullptr;
		}

		uint32 getId();

		bool update();

		Size getSize() const { return m_size; }

		Usage getUsage() const { return m_usage; }

	private:
		Size m_size;
		Usage m_usage;
		std::unordered_map<int32, std::shared_ptr<RHIResource>> m_colorAttachments;
		std::shared_ptr<RHIResource> m_depthAttachment;
		bool m_valid;
		uint32 m_id;
	};
}

#endif // !__volucris_rhi_frame_buffer_h__
