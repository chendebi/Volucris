#ifndef __volucris_frame_synthesizer_h__
#define __volucris_frame_synthesizer_h__

#include <Engine/Core/TypesHelp.h>
#include <atomic>

namespace volucris
{
	class FrameSynthesier
	{
	public:
		~FrameSynthesier();

		static FrameSynthesier& getInstance()
		{
			static FrameSynthesier inst;
			return inst;
		}

	protected:
		friend class Application;
		friend class Renderer;
		void waitRenderFrame(uint64 frame);

		void countGameFrame();

		void countRenderFrame()
		{
			m_renderFrameCount.fetch_add(1, std::memory_order_release);
		}

	protected:
		FrameSynthesier();
		FrameSynthesier(const FrameSynthesier&) = delete;
		FrameSynthesier(FrameSynthesier&&) = delete;
		FrameSynthesier& operator=(const FrameSynthesier&) = delete;
		FrameSynthesier& operator=(FrameSynthesier&&) = delete;

	private:
		uint64 m_gameFrameCount;
		std::atomic<uint64> m_renderFrameCount;
	};
}

#endif // !__volucris_frame_synthesizer_h__
