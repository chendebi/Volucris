#ifndef __volucris_rhi_resource_h__
#define __volucris_rhi_resource_h__

#include <Engine/Core/Object.h>
#include <Engine/Core/TypesHelp.h>
#include <functional>

namespace volucris
{
	struct RHIState;
	struct RHICommandList;

	class RHIResource : public Object
	{
	public:
		RHIResource() : Object(), m_id(0) {}

		~RHIResource() override;

		uint32 getId() const { return m_id; }

		bool isCreated() const { return m_id > 0; }

		virtual bool init(RHICommandList* command) = 0;

	protected:
		virtual uint32 create(RHIState* state) = 0;
		
		virtual void bind(RHIState* state) {}

		virtual void destroy(RHIState* state) = 0;

	private:
		friend class RHICommandList;
		uint32 m_id;
	};
}

#endif // !__volucris_rhi_resource_h__
