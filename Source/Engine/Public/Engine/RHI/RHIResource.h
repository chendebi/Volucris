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
		RHIResource() : Object() {}

		~RHIResource() override;
	};
}

#endif // !__volucris_rhi_resource_h__
