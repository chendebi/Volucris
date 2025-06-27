#include <Render/Frame.h>
#include <RHI/RHICommandList.h>
#include <RHI/RHIBuffer.h>
#include <RHI/RHIRenderTarget.h>
#include <Core/Volucris.h>
#include <Render/Renderer.h>

constexpr int FrameCount = 2;

namespace volucris
{
	Frame::Frame()
		: m_targets()
		, m_targetReaders()
		, m_targetData()
		, m_current(0)
	{
	}

	Frame::~Frame()
	{
		for (auto& target : m_targets)
		{
			RHICmdList->deleteResource(target.get());
		}

		for (auto& reader : m_targetReaders)
		{
			RHICmdList->deleteResource(reader.get());
		}
	}

	void Frame::resize(int width, int height)
	{
		for (auto& target : m_targets)
		{
			RHICmdList->deleteResource(target.get());
		}
		
		for (auto& reader : m_targetReaders)
		{
			RHICmdList->deleteResource(reader.get());
		}
		m_targetReaders.clear();

		RHITextureDesc desc;
		desc.pixelFormat = Texture::EPixelFormat::R8G8B8;

		auto size = 3 * width * height;
		m_targetData.format = getSourceFormat(desc.pixelFormat);
		m_targetData.size = { width, height };
		m_targetData.data.resize(size);

		for (int i = 0; i < FrameCount; ++i)
		{
			auto target = std::make_unique<RHIRenderTarget>(Size(width, height));
			target->attachColor(desc, 0);
			target->init(RHICmdList);
			m_targets.emplace_back(std::move(target));

			auto reader = std::make_unique<RHIReadPixelBuffer>(size);
			reader->init(RHICmdList);
			m_targetReaders.emplace_back(std::move(reader));
		}
		m_current = 0;
	}

	void Frame::render(RHICommandList* cmdList)
	{
		RENDER_SCOPE(Frame);

		cmdList->bindResource(m_targets[m_current].get());
		RHIClearState state;
		state.color = { 0.0, 0.0, 1.0, 1.0 };
		cmdList->clear(state);

		swapFrameData(cmdList);
	}

	void Frame::swapFrameData(RHICommandList* cmdList)
	{
		Rect rect = { 0,0,0,0 };
		rect.setSize(m_targets[m_current]->getSize());

		int next = (m_current + 1) % FrameCount;
		m_targetReaders[next]->startRead(cmdList, rect, m_targets[m_current].get(), 0);

		m_targetReaders[m_current]->readColorTo(m_targetData.data, cmdList);
		m_current = next;
	}
}