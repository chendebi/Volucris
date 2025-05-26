#ifndef __volucris_meta_data_h__
#define __volucris_meta_data_h__

#include <string>
#include <Engine/Core/serializer.h>

namespace volucris
{
	enum class ResourceType
	{
		UNKNOWN,
		MATERIAL = 1,
		STATIC_MESH
	};

	struct ResourceMeta
	{
		ResourceType type = ResourceType::UNKNOWN;
		std::string guid = std::string();
		std::string path = std::string();
		std::string sourcePath = std::string();

		void serialize(Serializer& serializer)
		{
			serializer.serialize((int)type);
			serializer.serialize(guid);
			serializer.serialize(path);
			serializer.serialize(sourcePath);
		}

		void deserialize(Serializer& serializer)
		{
			int typeValue;
			if (serializer.deserialize(typeValue) &&
				serializer.deserialize(guid) &&
				serializer.deserialize(path) &&
				serializer.deserialize(sourcePath))
			{
				type = (ResourceType)typeValue;
			}
		};

		bool isValid() const
		{
			return type != ResourceType::UNKNOWN;
		}
	};
}

#endif // !__volucris_meta_data_h__
