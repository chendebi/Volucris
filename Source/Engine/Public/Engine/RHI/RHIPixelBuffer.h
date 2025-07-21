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

		void bindTexture(const std::shared_ptr<RHITexture2D>& texture)
		{
			m_texture = texture;
		}

		void startRead(Rect rect);

		std::vector<uint8> readColor();

		bool readColorTo(std::vector<uint8>& data);

	private:
		std::shared_ptr<RHITexture2D> m_texture;
	};

	class RHIWritePixelBuffer : public RHIBuffer
	{
	public:
		RHIWritePixelBuffer(RHIBuffer::Usage usage = StaticDraw);

		void bindTexture(const std::shared_ptr<RHITexture2D>& texture)
		{
			m_texture = texture;
		}

		std::shared_ptr<RHITexture2D> getTexture() const { return m_texture; }

		void startWrite(std::vector<uint8> data);

		bool writeToTexture();

	private:
		std::shared_ptr<RHITexture2D> m_texture;
	};
}

#endif // !__volucris_rhi_pixelbuffer_h__
