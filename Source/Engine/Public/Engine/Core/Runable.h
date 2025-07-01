#ifndef __volucris_runable_h__
#define __volucris_runable_h__

#include <Engine/Core/CircleQueue.h>
#include <functional>
#include <thread>
#include <shared_mutex>
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

		void wait()
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

		// 确保前面的命令都执行完毕
		void flushCommands();

		virtual void run() = 0;

		void quit();

		bool isRunning() const
		{
			std::shared_lock lock(m_runningMutex);
			return m_running > 0;
		}

	protected:
		bool start(const std::function<void()>& main);

		virtual bool initialize() { return false; }

		virtual void destroy() {}

		void setRunning(bool running)
		{
			std::unique_lock lock(m_runningMutex);
			m_running = running;
		}

	protected:
		CircleQueue<std::function<void()>> m_queue;
		std::thread m_thread;
		mutable std::shared_mutex m_runningMutex;
		uint8 m_running;
		Fence* m_quitFence;
	};
}

#endif // !__volucris_runable_h__
