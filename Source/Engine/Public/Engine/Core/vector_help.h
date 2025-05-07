#ifndef __volucris_vector_help_h__
#define __volucris_vector_help_h__

#include <vector>
#include <functional>

namespace volucris
{
	namespace VectorHelp
	{
		template<typename T>
		inline bool quickRemove(std::vector<T>& vec, const T& val)
		{
			auto it = std::find(vec.begin(), vec.end(), val);
			if (it == vec.end())
			{
				return false;
			}
			std::swap(*it, *vec.rbegin());
			vec.pop_back();
			return true;
		}

		template<typename T>
		inline bool quickRemoveFirstIf(std::vector<T>& vec, std::function<bool(const T&)> pred)
		{
			auto it = std::find_if(vec.begin(), vec.end(), pred);
			if (it == vec.end())
			{
				return false;
			}
			std::swap(*it, *vec.rbegin());
			vec.pop_back();
			return true;
		}
	}
}

#endif // !__volucris_vector_help_h__
