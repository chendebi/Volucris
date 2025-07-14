#ifndef __volucris_rhi_vertex_buffer_h__
#define __volucris_rhi_vertex_buffer_h__

#include <Engine/RHI/RHIBuffer.h>

namespace volucris
{
	class RHIVertexBuffer : public RHIBuffer
	{
	public:
		enum DataType
		{
			Float,
		};
		struct Description
		{
			int location;
			DataType type;
			int size;
			bool normalized;
			int stride;
			int offset;
		};

	public:
		RHIVertexBuffer(RHICommandList* context, Usage usage = Usage::StaticDraw);

		void setDescriptions(const std::vector<Description>& descriptions)
		{
			m_descriptions = descriptions;
		}

		const std::vector<Description>& getDescriptions() const { return m_descriptions; }

	private:
		std::vector<Description> m_descriptions;
	};

	class RHIElementBuffer : public RHIBuffer
	{
	public:
		enum DataType
		{
			UByte,
			UShort,
			UInt
		};

		enum DrawMode
		{
			Points,
			LineStrip,
			LineLoop,
			Lines,
			LineStripAdjacency,
			LinesAdjacency,
			TraingleStrip,
			TraingleFan,
			Traingles
		};

	public:
		RHIElementBuffer(RHICommandList* context, Usage usage = Usage::StaticDraw);

		void init(const std::vector<uint16>& elements);

		void init(const std::vector<uint32>& elements);

		void init(const std::vector<uint8>& elements, DataType type=DataType::UByte);

		void init(const uint8* elements, size_t size, DataType type=DataType::UByte);

		void setDrawMode(DrawMode mode) { m_mode = mode; }

		DataType getDataType() const { return m_type; }

		uint32 getCount() const { return m_count; }

		DrawMode getDrawMode() const { return m_mode; }

	private:
		DataType m_type;
		uint32 m_count;
		DrawMode m_mode;
	};
}

#endif // !__volucris_rhi_vertex_buffer_h__
