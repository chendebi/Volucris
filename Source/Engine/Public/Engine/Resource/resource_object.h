#ifndef __volucris_resource_object_h__
#define __volucris_resource_object_h__

#include "Engine/Resource/resource_path.h"

namespace volucris
{
	class ResourceObject
	{
	public:
		ResourceObject() = default;

		virtual ~ResourceObject() = default;

		const ResourcePath& getResourcePath() const { return m_path; }

	private:
		friend class ResourceManager;
		ResourcePath m_path;
	};
}

#endif // !__volucris_resource_object_h__
