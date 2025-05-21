#include "Resource/resource_object.h"
#include "Resource/resource_registry.h"

namespace volucris
{
	std::shared_ptr<ResourceObject> ResourceObject::loadDependence(const rapidjson::Value& serializer, const char* key) const
	{
		auto it = serializer.FindMember(key);
		if (it != serializer.MemberEnd() && it->value.IsString())
		{
			return ResourceRegistry::Instance().loadResourceByGUID(GUID(it->value.GetString()));
		}
		return nullptr;
	}
}
