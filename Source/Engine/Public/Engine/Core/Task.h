#ifndef __volucris_task_h__
#define __volucris_task_h__

#include <functional>
#include <Engine/Core/TypesHelp.h>

namespace volucris
{
	using Task = std::function<void()>;

	template <typename T>
		class __v_is_task_class{
			template <typename U>
			static auto test(int) -> decltype(std::declval<U>().execute(), std::true_type());
			template <typename>
			static std::false_type test(...);
		public:
			static constexpr bool value = decltype(test<T>(0))::value;
	};

	template<typename T>
	inline Task createTask(T&& task)
	{
		static_assert(__v_is_task_class<T>::value);
		return [func = std::forward<T>(task)]() mutable {
			func.execute();
			};
	}
}

#endif // !__volucris_task_h__
