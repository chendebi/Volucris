#ifndef __volucris_rhi_resource_h__
#define __volucris_rhi_resource_h__

#include <Engine/Core/Object.h>
#include <Engine/Core/TypesHelp.h>
#include <functional>

namespace volucris
{
	class RHICommandList;

	class RHIResource : public Object
	{
	public:
		RHIResource(RHICommandList* context = nullptr);

		~RHIResource() override;

		RHICommandList* getContext() const { return m_context; }

		void setContext(RHICommandList* context);

	protected:
		RHICommandList* m_context;
	};
}

#endif // !__volucris_rhi_resource_h__
