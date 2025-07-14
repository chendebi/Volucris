#include <RHI/RHIResource.h>
#include <Core/Assert.h>
#include <Core/Volucris.h>

namespace volucris
{
	RHIResource::RHIResource(RHICommandList* context)
		: Object()
		, m_context(context)
	{

	}

	RHIResource::~RHIResource()
	{
		
	}

	void RHIResource::setContext(RHICommandList* context)
	{
		v_check(m_context == nullptr);
		m_context = context;
	}
}