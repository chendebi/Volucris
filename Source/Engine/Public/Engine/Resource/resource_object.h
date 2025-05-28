#ifndef __volucris_resource_object_h__
#define __volucris_resource_object_h__

#include <vector>
#include <string>
#include "Engine/Resource/resource_path.h"
#include <Engine/Core/serializer.h>
#include <rapidjson/document.h>
#include "Engine/Resource/meta_data.h"

namespace volucris
{
	class ResourceObject
	{
	public:
		ResourceObject() = default;

		virtual ~ResourceObject() = default;

		const ResourcePath& getResourcePath() const { return m_path; }

		const ResourceMeta& getMetaData() const { return m_metaData; }

		void setResourceName(const std::string& name);

	protected:
		friend class ResourceRegistry;
		virtual bool serialize(Serializer& serializer) const { return false; }
		
		virtual void deserialize(Serializer& serializer) { }

		void setResourcePath(const ResourcePath& path) { m_path = path; }

	private:
		ResourcePath m_path;
		ResourceMeta m_metaData;
	};
}

#endif // !__volucris_resource_object_h__
