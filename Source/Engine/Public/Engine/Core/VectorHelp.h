#ifndef __volucris_vector_help_h__
#define __volucris_vector_help_h__

#include <vector>
#include <memory>

namespace volucris
{
	namespace VectorHelp
	{
		template<typename T>
		inline bool quickRemove(std::vector<std::shared_ptr<T>>& vec, T* value)
		{
			auto it = std::find_if(vec.begin(), vec.end(), [value](const std::shared_ptr<T>& ptr)->bool {
				if (ptr.get() == value)
				{
					return true;
				}
				return false;
				});
			if (it != vec.end())
			{
				std::swap(*it, *(vec.rbegin()));
				vec.pop_back();
				return true;
			}
			return false;
		}

		template<typename T>
		inline bool quickRemove(std::vector<T>& vec, const T& value)
		{
			auto it = std::find(vec.begin(), vec.end(), value);
			if (it != vec.end())
			{
				std::swap(*it, *(vec.rbegin()));
				vec.pop_back();
				return true;
			}
			return false;
		}
	}
}

#endif // !__volucris_vector_help_h__
