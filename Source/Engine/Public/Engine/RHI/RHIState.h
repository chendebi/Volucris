#ifndef __volucris_rhi_state_h__
#define __volucris_rhi_state_h__

#include <glm/glm.hpp>
#include <Engine/Core/Rect.h>
#include <Engine/Core/TypesHelp.h>

namespace volucris
{
	class RHITexture;
	class RHIRenderTarget;

	struct RHIClearState
	{
		enum Buffer
		{
			ColorBuffer = 0x01,
			DepthBuffer = 0x02,
			ColorAndDepth = ColorBuffer | DepthBuffer,
			All = ColorBuffer | DepthBuffer,
		};

		uint32 buffers = Buffer::ColorAndDepth;
		glm::vec4 color = glm::vec4(1,1,0,1);
	};

	struct RHIState
	{
		Rect viewport = { 0,0,0,0 };
		RHIClearState clearState = RHIClearState();
		RHITexture* texture = nullptr;
		RHITexture* texture2d = nullptr;
		RHIRenderTarget* renderTarget = nullptr;
	};
}

#endif // !__volucris_rhi_state_h__
