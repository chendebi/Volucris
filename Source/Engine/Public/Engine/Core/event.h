#ifndef __volucris_event_h__
#define __volucris_event_h__

#include <functional>
#include <vector>
#include <type_traits>

namespace volucris
{
	template <typename ...Args>
	class Event
	{
	public:
		virtual ~Event() = default;

		// 绑定Lamda表达式
		size_t addLambda(std::function<void(Args...)> lamda)
		{
			m_callables.push_back({ m_id, std::move(lamda) });
			auto id = m_id;
			++m_id;
			return id;
		}

		bool remove(size_t id)
		{
			auto it = std::find(m_callables.begin(), m_callables.end(), id);
			if (it == m_callables.end())
			{
				return true;
			}
			it->func = nullptr;
			std::swap(*it, *(m_callables.rbegin()));
			m_callables.pop_back();
		}

		// 绑定成员函数
		template <typename T, typename Method>
		void addObject(T* object, Method method)
		{
			m_objectCallables.push_back({ object, [object, method](Args... args) {
				// 使用 std::invoke 调用成员函数
				std::invoke(method, object, std::forward<Args>(args)...);
				} });
		}

		template <typename T>
		void removeAll(T* object)
		{
			auto i = 0;
			bool finish = false;
			while (!finish)
			{
				if (i == m_objectCallables.size())
				{
					break;
				}

				if (m_objectCallables[i].object != object)
				{
					++i;
					continue;
				}

				auto it = m_objectCallables.begin() + i;
				std::swap(*it, *(m_objectCallables.rbegin()));
				m_objectCallables.pop_back();
			}
		}

		void broadcast(Args ...args)
		{
			for (auto& callable : m_callables)
			{
				callable.call(std::forward<Args>(args)...);
			}

			for (auto& callable : m_objectCallables)
			{
				callable.call(std::forward<Args>(args)...);
			}
		}

		void operator()(Args ...args)
		{
			broadcast(std::forward<Args>(args)...);
		}

	private:
		struct Callable
		{
			size_t id;
			std::function<void(Args...)> func;

			bool operator==(size_t id) const
			{
				return this->id == id;
			}

			void call(Args... args)
			{
				func(std::forward<Args>(args)...);
			}
		};

		struct ObjectCallable
		{
			void* object;
			std::function<void(Args...)> func;

			void call(Args... args)
			{
				func(std::forward<Args>(args)...);
			}
		};

		size_t m_id = 0;
		std::vector<Callable> m_callables;
		std::vector<ObjectCallable> m_objectCallables;
	};

	enum Key
	{
		KEY_A = 'A', KEY_B, KEY_C, KEY_D, KEY_E, KEY_F, KEY_G, KEY_H, KEY_I, KEY_J, KEY_K, KEY_L, KEY_M, KEY_N,
		KEY_0, KEY_P, KEY_Q, KEY_R, KEY_S, KEY_T, KEY_U, KEY_V, KEY_W, KEY_X, KEY_Y, KEY_Z,
		KEY_F1 = 0x70, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_F11, KEY_F12
	};
}

#define DECLARE_EVENT(EVENT_TYPE, ...) class EVENT_TYPE : public volucris::Event<##__VA_ARGS__> {};

#endif // !__volucris_event_h__
