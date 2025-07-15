#ifndef __volucris_circle_buffer_h__
#define __volucris_circle_buffer_h__

#include <limits>

namespace volucris
{
	template <typename T>
	class CircleBuffer
	{
	public:
		struct Iterator
		{
			Iterator(CircleBuffer<T>* buffer, size_t idx = std::numeric_limits<size_t>::max())
				: m_index(idx)
				, m_buffer(buffer)
			{
			}

			Iterator() : Iterator(nullptr) {}

			bool operator==(const Iterator& it) const
			{
				return it.m_index == m_index;
			}

			bool operator!=(const Iterator& it) const
			{
				return !operator==(it);
			}

			Iterator& operator++()
			{
				++m_index;
				return *this;
			}

			Iterator& operator--()
			{
				--m_index;
				return *this;
			}

			T& operator*()
			{
				auto idx = (m_buffer->m_head + m_index) % m_buffer->m_capacity;
				return m_buffer->m_buffer[idx];
			}

		private:
			size_t m_index;
			CircleBuffer* m_buffer;
		};

	public:
		CircleBuffer(size_t capacity)
			: m_capacity(capacity)
			, m_size(0)
			, m_head(0)
			, m_tail(0)
			, m_buffer(new T[capacity])
		{

		}

		~CircleBuffer()
		{
			delete[] m_buffer;
		}

		void push(const T& val)
		{
			if (m_size == m_capacity)
			{
				pop();
			}
			m_buffer[m_tail] = val;
			m_tail = (m_tail + 1) % m_capacity;
			m_size++;
		}

		void pop(T* val=nullptr)
		{
			if (val)
			{
				*val = m_buffer[m_head];
			}
			m_buffer[m_head] = T();
			m_head = (m_head + 1) % m_capacity;
			m_size--;
		}

		size_t count() const { return m_size; }

		Iterator begin()
		{
			return Iterator(this, 0);
		}

		Iterator end()
		{
			return Iterator(this, m_size);
		}

	private:
		size_t m_capacity;
		size_t m_size;
		size_t m_head;
		size_t m_tail;
		T* m_buffer;
	};
}

#endif // !__volucris_circle_buffer_h__
