#ifndef __volucris_view_h__
#define __volucris_view_h__

#include <memory>
#include <Engine/RHI/RHICommandList.h>
#include <Engine/Core/TextureDefines.h>

namespace volucris
{
	class RHIRenderTarget;
	class RHIReadPixelBuffer;

	class View
	{
	public:
		View();

		~View();

		void resize(int width, int height);

		void render(RHICommandList* cmdList);

		void swapViewData(RHICommandList* cmdList);

	private:
		std::vector<std::unique_ptr<RHIRenderTarget>> m_targets;
		std::vector<std::unique_ptr<RHIReadPixelBuffer>> m_targetReaders;
		Texture::TextureData m_targetData;
		int m_current;
	};
}

#endif // !__volucris_view_h__
