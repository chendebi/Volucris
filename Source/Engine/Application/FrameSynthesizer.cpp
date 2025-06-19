#include "Application/FrameSynthesizer.h"
#include <thread>
#include <Core/Volucris.h>
#include <Profile/Profile.h>

namespace volucris
{
	FrameSynthesier::FrameSynthesier()
		: m_gameFrameCount(0)
		, m_renderFrameCount(0)
	{
	}

	FrameSynthesier::~FrameSynthesier()
	{
	}

	void FrameSynthesier::countGameFrame()
	{
		V_SCOPED_PROFILE
		waitRenderFrame(m_gameFrameCount);
		++m_gameFrameCount;
	}

	void FrameSynthesier::waitRenderFrame(uint64 frame)
	{
		while (m_renderFrameCount.load(std::memory_order_acquire) < frame)
		{
			std::this_thread::yield();
		}
	}
}
