#include "Core/Runable.h"
#include <Core/Volucris.h>
#include <Application/FrameSynthesizer.h>

namespace volucris
{
	Runable::Runable(size_t queueSize)
		: m_queue(queueSize)
		, m_thread()
		, m_running(false)
		, m_quitFence(nullptr)
	{
	}

	Runable::~Runable()
	{
	}

	void Runable::push(std::function<void()> cmd, bool block)
	{
		m_queue.push(std::move(cmd), block);
	}

	void Runable::quit()
	{
		Fence fence;
		m_queue.push([this, &fence]() {
			m_running = false;
			m_quitFence = &fence;
			});
		fence.waite();
	}

	bool Runable::start(const std::function<void()>& main)
	{
		m_thread = std::thread([this, main]() {
			// 初始化
			V_LOG_INFO(Engine, "start thread")
			m_running = initialize();
			if (!m_running)
			{
				return;
			}

			bool render = false;
			std::function<void()> msg;
			while (m_running)
			{
				main();
				
				while (m_queue.pop(msg, true))
				{
					if (!msg)
					{
						break;
					}
					msg();

					if (!m_running)
					{
						break;
					}
				}
			}

			V_LOG_INFO(Engine, "destroy thread")
			destroy();
			V_LOG_INFO(Engine, "quit thread")
			m_quitFence->signal();
			});
		m_thread.detach();
		return true;
	}
}

