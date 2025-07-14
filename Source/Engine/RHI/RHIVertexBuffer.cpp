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
	{
		setContext(context);
	}

	void RHIElementBuffer::init(const std::vector<uint32>& elements)
	{
		const uint8* data = (uint8*)elements.data();
		uint32 size = elements.size() * sizeof(uint32);
		RHIBuffer::init(data, size);
	}
}
