#ifndef __volucris_frame_h__
#define __volucris_frame_h__

#include <memory>
#include <Engine/RHI/RHICommandList.h>
#include <Core/TextureDefines.h>

namespace volucris
{
	class RHIRenderTarget;
	class RHIReadPixelBuffer;

	class Frame
	{
	public:
		Frame();

		~Frame();

		void resize(int width, int height);

		void render(RHICommandList* cmdList);

		void swapFrameData(RHICommandList* cmdList);

	private:
		std::vector<std::unique_ptr<RHIRenderTarget>> m_targets;
		std::vector<std::unique_ptr<RHIReadPixelBuffer>> m_targetReaders;
		Texture::TextureData m_targetData;
		int m_current;
	};
}

#endif // !__volucris_frame_h__
