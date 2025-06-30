#ifndef __volucris_rhi_buffer_h__
#define __volucris_rhi_buffer_h__

#include <Engine/RHI/RHIResource.h>
#include <Engine/Core/TextureDefines.h>
#include <Engine/Core/Rect.h>

namespace volucris
{
	class RHITexture2D;
	class RHIRenderTarget;
	class RHICommandList;

	class RHIBuffer : public RHIResource
	{
	public:
		enum EBufferUsage
		{
			StaticDraw,
			DynamicDraw,
			StreamRead,
			StreamWrite
		};

		bool init(RHICommandList* command) override;

	protected:
		struct Impl;
		RHIBuffer(std::unique_ptr<Impl> buffer);

		uint32 create(RHIState* state) override;

		void bind(RHIState* state) override;

		void destroy(RHIState* state) override;

	protected:
		std::unique_ptr<Impl> m_impl;
	};

	class RHIReadPixelBuffer : public RHIBuffer
	{
	public:
		RHIReadPixelBuffer(size_t size, EBufferUsage usage = StaticDraw);

		void startRead(RHICommandList* command, Rect rect, RHIRenderTarget* renderTarget, int index = 0);

		std::vector<uint8> readColor(RHICommandList* command);

		bool readColorTo(std::vector<uint8>& data, RHICommandList* command);
	};

	class RHIWritePixelBuffer : public RHIBuffer
	{
	public:
		RHIWritePixelBuffer(size_t size, EBufferUsage usage = StaticDraw);

		void startWrite(RHICommandList* command, std::vector<uint8> data);

		std::vector<uint8> readColor(RHICommandList* command);

		bool writeTo(RHITexture2D* texture, RHICommandList* command);
	};
}

#endif // !__volucris_rhi_buffer_h__
