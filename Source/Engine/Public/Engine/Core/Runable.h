#ifndef __volucris_runable_h__
#define __volucris_runable_h__

#include <Engine/Core/CircleQueue.h>
#include <functional>
#include <thread>
#include <Engine/Core/TypesHelp.h>

namespace volucris
{
	struct Fence
	{
	public:
		void signal()
		{
			std::lock_guard lock(m_mutex);
			m_ready = true;
			m_condition.notify_all();
		}

		void waite()
		{
			std::unique_lock lock(m_mutex);
			m_condition.wait(lock, [this] {return m_ready; });
		}

	private:
		bool m_ready = false;
		std::mutex m_mutex;
		std::condition_variable m_condition;
	};

	class Runable
	{
	public:
		Runable(size_t queueSize);

		virtual ~Runable();

		void push(std::function<void()> cmd, bool block=true);

		virtual void run() = 0;

		void quit();

	protected:
		bool start(const std::function<void()>& main);

		virtual bool initialize() { return false; }

		virtual void destroy() {}

	protected:
		CircleQueue<std::function<void()>> m_queue;
		std::thread m_thread;
		uint8 m_running;
		Fence* m_quitFence;
	};
}

#endif // !__volucris_runable_h__
