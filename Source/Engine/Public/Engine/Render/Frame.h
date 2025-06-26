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

		void resize(int width, int height);

		void render(RHICommandList* cmdList);

	private:
		std::unique_ptr<RHIRenderTarget> m_target;
		std::vector<std::unique_ptr<RHIReadPixelBuffer>> m_targetReaders;
		std::vector<Texture::TextureData> m_targetDatas;
	};
}

#endif // !__volucris_frame_h__
