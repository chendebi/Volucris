#ifndef __volucris_circle_queue_h__
#define __volucris_circle_queue_h__

namespace volucris
{
	template <typename T>
	class CircleQueue
	{
		size_t m_size;
		size_t m_head;
		size_t m_tail;
		size_t m_count;
		T* m_data;

	public:
		CircleQueue(const CircleQueue&) = delete;

	public:
		CircleQueue(size_t size)
			: m_size(size)
			, m_head(0)
			, m_tail(0)
			, m_count(0)
			, m_data(new T[size])
		{
		}

		~CircleQueue()
		{
			delete[] m_data;
		}

		bool push(const T& value)
		{
			if (m_count == m_size)
			{
				return false;
			}

			m_data[m_tail] = value;
			m_tail = (m_tail + 1) % m_size;
			m_count++;
			return true;
		}

		bool isEmpty() const
		{
			return m_count == 0;
		}

		bool pop(T& value)
		{
			if (m_count == 0)
			{
				return false;
			}
			value = m_data[m_head];
			m_data[m_head] = T(); // Optional: clear the value
			m_head = (m_head + 1) % m_size;
			m_count--;
			return true;
		}
	};
}

#endif // !__volucris_circle_queue_h__
