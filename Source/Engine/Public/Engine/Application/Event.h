#ifndef __volucris_event_h__
#define __volucris_event_h__

#include <functional>
#include <vector>
#include <type_traits>

namespace volucris
{
	template <typename ReturnType, typename ...Args>
	class EventCallable
	{
	public:
		virtual ReturnType invoke(Args... args) = 0;
	};

	template <typename Callable, typename ReturnType, typename ...Args>
	class EventCallableWrapper
	{
	public:
		EventCallableWrapper(const Callable& callable)
			: m_callable(callable) {}

		EventCallableWrapper(Callable&& callable)
			: m_callable(std::move(callable)) 
		{
		}
		ReturnType invoke(Args... args)
		{
			return m_callable.invoke(std::forward<Args>(std::move(args))...);
		}

	private:
		Callable m_callable;
	};

	template <typename ReturnType, typename ...Args>
	class EventDelegate
	{
		using EventCallablePtr = EventCallable<ReturnType, Args...>*;

		EventCallablePtr m_callable = nullptr;

	public:
		EventDelegate() = default;

		template<typename Callable>
		EventDelegate(Callable&& callable)
		{
			m_callable = new EventCallableWrapper<std::decay_t<Callable>, ReturnType, Args...>(std::forward<Callable>(callable));
		}

		EventDelegate(const EventDelegate& other)
			: m_callable(other.m_callable)
		{
		}

		EventDelegate(EventDelegate&& other)
			: m_callable(other.m_callable)
		{
			other.m_callable = nullptr;
		}

		~EventDelegate()
		{
			delete m_callable;
		}

		ReturnType invoke(Args... args)
		{
			if (m_callable)
			{
				return m_callable->invoke(std::forward<Args>(args)...);
			}
			return ReturnType();
		}

		bool isValid() const { return m_callable != nullptr; }
	};


	template <typename ReturnType, typename ...Args>
	class EventMutiDelegate
	{
		using EventCallablePtr = EventCallable<ReturnType, Args...>*;

		std::vector<EventCallablePtr> m_callables;

	public:
		EventMutiDelegate() = default;

		EventDelegate()
		{

		}

		EventDelegate(const EventDelegate& other)
			: m_callable(other.m_callables)
		{
		}

		EventDelegate(EventDelegate&& other)
			: m_callable(std::move(other.m_callables))
		{
			other.m_callable = nullptr;
		}

		~EventDelegate()
		{
			delete m_callable;
		}

		template<typename Callable>
		void addCallable(EventCallablePtr callable)
		{
			if (callable)
			{
				m_callables.push_back(callable);
			}
		}

		ReturnType invoke(Args... args)
		{
			if (m_callable)
			{
				return m_callable->invoke(std::forward<Args>(args)...);
			}
			return ReturnType();
		}

		bool isValid() const { return m_callable != nullptr; }
	};

	

	enum class Key
	{
		KEY_INVALID = 0,
		KEY_A = 'A', KEY_B, KEY_C, KEY_D, KEY_E, KEY_F, KEY_G, KEY_H, KEY_I, KEY_J, KEY_K, KEY_L, KEY_M, KEY_N,
		KEY_0, KEY_P, KEY_Q, KEY_R, KEY_S, KEY_T, KEY_U, KEY_V, KEY_W, KEY_X, KEY_Y, KEY_Z,
		KEY_F1 = 0x70, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_F11, KEY_F12
	};

	enum class Modifier
	{
		SHIFT = 0x01,
		CTRL = 0x02,
		ALT = 0x04
	};

}

#define DECLARE_EVENT(EVENT_TYPE, ...) class EVENT_TYPE : public volucris::Event<##__VA_ARGS__> {};

#endif // !__volucris_event_h__