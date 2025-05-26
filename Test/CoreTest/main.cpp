#include <Engine/Core/serializer.h>
#include <string>
#include <iostream>
#include <Engine/Resource/meta_data.h>

using namespace volucris;

int main()
{
	if constexpr (has_serialize_method<ResourceMeta>::value) {
		std::cout << "find function \n";
	}
	else
	{
		std::cout << "not find function \n";
	}
	Serializer serializer;
	{
		ResourceMeta meta;
		meta.guid = "this is guid";
		meta.path = "this is path var";
		meta.type = ResourceType::MATERIAL;
		meta.sourcePath = "/////ssx";
		serializer.serialize(meta);
	}

	{
		ResourceMeta meta;
		serializer.deserialize(meta);
	}
}