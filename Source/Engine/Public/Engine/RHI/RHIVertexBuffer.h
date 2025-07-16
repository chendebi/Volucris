#ifndef __volucris_rhi_vertex_buffer_h__
#define __volucris_rhi_vertex_buffer_h__

#include <Engine/RHI/RHIBuffer.h>
#include <Engine/Core/DataType.h>

namespace volucris
{
	class RHIVertexBuffer : public RHIBuffer
	{
	public:
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
		RHIElementBuffer(RHICommandList* context, Usage usage = Usage::StaticDraw);

	};
}

#endif // !__volucris_rhi_vertex_buffer_h__
