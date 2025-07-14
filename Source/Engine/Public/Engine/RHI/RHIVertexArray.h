#ifndef __volucris_rhi_vertex_array_h__
#define __volucris_rhi_vertex_array_h__

#include <Engine/RHI/RHIResource.h>

namespace volucris
{
	class RHIVertexBuffer;

	class RHIVertexArray : public RHIResource
	{
	public:
		RHIVertexArray(RHICommandList* context);

		~RHIVertexArray();

		void init(const std::shared_ptr<RHIVertexBuffer>& buffer);

		uint32 getId() const { return m_id; }

	private:
		std::shared_ptr<RHIVertexBuffer> m_buffer;
		uint32 m_id;
	};
}

#endif // !__volucris_rhi_vertex_array_h__
