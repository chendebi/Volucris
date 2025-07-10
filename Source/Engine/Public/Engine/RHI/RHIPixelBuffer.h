#ifndef __volucris_rhi_pixelbuffer_h__
#define __volucris_rhi_pixelbuffer_h__

#include <Engine/RHI/RHIBuffer.h>

namespace volucris
{
	class RHIRenderTarget;
	class RHITexture2D;

	class RHIReadPixelBuffer : public RHIBuffer
	{
	public:
		RHIReadPixelBuffer(RHIBuffer::Usage usage = StaticDraw);

		void startRead(Rect rect, RHIRenderTarget* renderTarget, int index = 0);

		std::vector<uint8> readColor();

		bool readColorTo(std::vector<uint8>& data);
	};

	class RHIWritePixelBuffer : public RHIBuffer
	{
	public:
		RHIWritePixelBuffer(RHIBuffer::Usage usage = StaticDraw);

		void startWrite(std::vector<uint8> data);

		bool writeTo(RHITexture2D* texture);
	};
}

#endif // !__volucris_rhi_pixelbuffer_h__
