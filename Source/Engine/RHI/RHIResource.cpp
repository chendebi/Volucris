#include <RHI/RHIResource.h>
#include <Core/Assert.h>
#include <Core/Volucris.h>

namespace volucris
{
	RHIResource::~RHIResource()
	{
		v_checkf(!isCreated(), Engine, "rhi resource not destroyed by command list");
	}
}