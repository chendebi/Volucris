#ifndef __volucris_object_h__
#define __volucris_object_h__

#include <memory>

namespace volucris
{
	class Object : public std::enable_shared_from_this<Object>
	{
	public:
		Object() : std::enable_shared_from_this<Object>()
		{
		}

		virtual ~Object() = default;

		template<typename T>
		std::shared_ptr<T> getShared()
		{
			return std::static_pointer_cast<T>(shared_from_this());
		}
	};
}

#endif // !__volucris_object_h__
