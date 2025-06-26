#include <Render/Frame.h>
#include <RHI/RHICommandList.h>
#include <RHI/RHIBuffer.h>
#include <RHI/RHIRenderTarget.h>

constexpr int FrameCount = 2;

namespace volucris
{
	Frame::Frame()
		: m_target(nullptr)
		, m_targetReaders()
		, m_targetDatas()
	{
	}

	void Frame::resize(int width, int height)
	{
		RHICmdList.deleteResource(m_target.get());
		for (auto& reader : m_targetReaders)
		{
			RHICmdList.deleteResource(reader.get());
		}
		m_targetDatas.clear();
		m_targetReaders.clear();

		m_target = std::make_unique<RHIRenderTarget>(Size(width, height));
		RHITextureDesc desc;
		desc.pixelFormat = Texture::EPixelFormat::R8G8B8;
		m_target->attachColor(desc, 0);

		auto size = 3 * width * height;
		auto format = getSourceFormat(desc.pixelFormat);
		for (int i = 0; i < FrameCount; ++i)
		{
			auto target = std::make_unique<RHIReadPixelBuffer>(size);
			m_targetReaders.emplace_back(std::move(target));
			Texture::TextureData texture;
			texture.format = format;
			texture.size = { width, height };
		}
	}
}