#ifndef __volucris_timer_h__
#define __volucris_timer_h__

#include <chrono>

namespace volucris
{
	using TimePoint = std::chrono::time_point<std::chrono::high_resolution_clock>;

	class Timer
	{
	public:
		Timer();

		void start();

		void stop();

		TimePoint getStartTime() const { return m_start; }

		TimePoint getStopTime() const { return m_stop; }

		bool isRunning() const { return m_running; }

		double getDuration() const;

	private:
		TimePoint m_start;
		TimePoint m_stop;
		bool m_running;
	};
}

#endif // !__volucris_timer_h__
