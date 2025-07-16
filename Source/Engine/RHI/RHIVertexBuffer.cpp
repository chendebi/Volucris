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
}
