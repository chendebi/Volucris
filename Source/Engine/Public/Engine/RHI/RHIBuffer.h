#ifndef __volucris_rhi_buffer_h__
#define __volucris_rhi_buffer_h__

#include <Engine/RHI/RHIResource.h>
#include <Engine/Core/TextureDefines.h>
#include <Engine/Core/Rect.h>

namespace volucris
{
	class RHIOpenGLBuffer;

	class RHIBuffer : public RHIResource
	{
	public:
		enum Type
		{
			VertexBuffer,
			IndexBuffer,
			PixelPackBuffer,
			PixelUnpackBuffer,
		};

		enum Usage
		{
			StaticDraw,
			DynamicDraw,
			StreamRead,
			StreamWrite
		};

		RHIBuffer(Type type, Usage usage);

		~RHIBuffer() override;

		void init(uint64 bufferSize);

		void init(const std::vector<uint8>& data);

		Type getType() const { return m_type; }

	protected:
		Type m_type;
		Usage m_usage;
		std::unique_ptr<RHIOpenGLBuffer> m_buffer;
	};

}

#endif // !__volucris_rhi_buffer_h__
