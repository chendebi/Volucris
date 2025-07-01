#include "Profile/Timer.h"

namespace volucris
{
	Timer::Timer()
		: m_start()
		, m_stop()
		, m_running(false)
	{
	}

	void Timer::start()
	{
		m_start = std::chrono::high_resolution_clock::now();
		m_running = true;
	}

	void Timer::stop()
	{
		m_stop = std::chrono::high_resolution_clock::now();
		m_running = false;
	}
	double Timer::getDuration() const
	{
		return std::chrono::duration<double>(m_stop - m_start).count();
	}
}
