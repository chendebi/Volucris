#ifndef __volucris_circle_queue_h__
#define __volucris_circle_queue_h__

#include <condition_variable>
#include <shared_mutex>

namespace volucris
{
	template<typename T>
	class CircleQueue
	{
	public:
		CircleQueue(size_t capacity)
			: m_capacity(capacity)
			, m_count(0)
			, m_head(0)
			, m_tail(0)
			, m_buffer(new T[capacity])
			, m_mutex()
			, m_notFull()
			, m_notEmpty()
		{

		}

		bool push(const T& val, bool block = true)
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			if (block)
			{
				while (m_count == m_capacity)
				{
					m_notFull.wait(lock);
				}
			}
			else if (m_count == m_capacity)
			{
				return false;
			}

			m_buffer[m_tail] = val;
			m_tail = (m_tail+1) % m_capacity;
			m_count++;
			m_notEmpty.notify_one();
		}

		bool push(T&& val, bool block = true)
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			if (block)
			{
				while (m_count == m_capacity)
				{
					m_notFull.wait(lock);
				}
			}
			else if (m_count == m_capacity)
			{
				return false;
			}

			m_buffer[m_tail] = std::move(val);
			m_tail = (m_tail + 1) % m_capacity;
			m_count++;
			m_notEmpty.notify_one();
		}

		bool pop(T& val, bool block=true)
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			if (block)
			{
				while (m_count == 0)
				{
					m_notEmpty.wait(lock);
				}
			}
			else if (m_count == 0)
			{
				return false;
			}

			val = std::move(m_buffer[m_head]);
			m_buffer[m_head] = T();
			m_head = (m_head + 1) % m_capacity;
			m_count--;
			return true;
		}

	private:
		const size_t m_capacity;
		size_t m_count;
		size_t m_head;
		size_t m_tail;
		T* m_buffer;
		std::mutex m_mutex;
		std::condition_variable m_notFull;
		std::condition_variable m_notEmpty;
	};
}

#endif // !__volucris_circle_queue_h__
