#ifndef __volucris_shareable_object_h__
#define __volucris_shareable_object_h__

#include <memory>

namespace volucris
{
	class ShareableObject : std::enable_shared_from_this<ShareableObject>
	{
	public:
		virtual ~ShareableObject() = default;

		template<typename T>
		std::shared_ptr<T> getShared()
		{
			return std::dynamic_pointer_cast<T>(shared_from_this());
		}
	};
}

#endif // !__volucris_shareable_object_h__
