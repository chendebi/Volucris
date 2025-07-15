#include "RHI/RHIVertexBuffer.h"

namespace volucris
{
	RHIVertexBuffer::RHIVertexBuffer(RHICommandList* context, Usage usage)
		: RHIBuffer(RHIBuffer::VertexBuffer, usage)
	{
		setContext(context);
	}

	RHIElementBuffer::RHIElementBuffer(RHICommandList* context, Usage usage)
		: RHIBuffer(RHIBuffer::ElementBuffer, usage)
		, m_type(ElementDataType::UByte)
		, m_count(0)
		, m_mode(Traingles)
	{
		setContext(context);
	}

	void RHIElementBuffer::init(const std::vector<uint16>& elements)
	{
		init((uint8*)elements.data(), elements.size() * sizeof(uint16), ElementDataType::UShort);
	}

	void RHIElementBuffer::init(const std::vector<uint32>& elements)
	{
		init((uint8*)elements.data(), elements.size() * sizeof(uint32), ElementDataType::UInt);
	}

	void RHIElementBuffer::init(const std::vector<uint8>& elements, ElementDataType type)
	{
		init(elements.data(), elements.size(), type);
	}

	void RHIElementBuffer::init(const uint8* elements, size_t size, ElementDataType type)
	{
		m_type = type;
		RHIBuffer::init(elements, size);
		switch (type)
		{
		case volucris::ElementDataType::UByte:
			m_count = size;
			break;
		case volucris::ElementDataType::UShort:
			m_count = size / 2;
			break;
		case volucris::ElementDataType::UInt:
			m_count = size / 4;
			break;
		default:
			break;
		}
	}
}
