#ifndef __volucris_asset_path_h__
#define __volucris_asset_path_h__

#include <string>
#include <memory>
#include <xhash>
#include <fstream>
#include <Engine/Core/serializer.h>

namespace volucris
{
	struct UUID
	{
		std::string value;

		UUID() : value() {}

		explicit UUID(const ::std::string& uid)
			: value(uid)
		{
		}

		bool operator==(const UUID& other) const
		{
			return other.value == value;
		}

		bool operator!=(const UUID& other) const
		{
			return other.value != value;
		}

		bool operator<(const UUID& other) const
		{
			return value < other.value;
		}

		bool valid() const
		{
			return !value.empty();
		}

		void serialize(Serializer& serializer)
		{
			serializer.serialize(value);
		}

		void deserialize(Serializer& serializer)
		{
			serializer.deserialize(value);
		}

		static UUID generate();
	};
}

namespace std
{
	template<>
	struct hash<volucris::UUID>
	{
		inline size_t operator()(const volucris::UUID& id) const
		{
			return std::hash<std::string>{}(id.value);
		}
	};
}

namespace volucris
{
	class ResourceObject;

	// /Engine/Material/M_Red.mat
	struct Asset
	{
		enum Type
		{
			UNKNOWN,
			MATERIAL,
			TEXTURE,
			STATIC_MESH,
			CUSTOM = 16
		};

		UUID uuid;
		std::string path;
		std::string name;
		std::string sourcePath;
		std::string assetPath;

		Asset()
			: uuid()
			, path()
			, name()
			, sourcePath()
		{
		}

		Asset(const std::string& pathName);

		std::string getAssetPath() const;

		std::shared_ptr<ResourceObject> load();

		void serialize(Serializer& serializer)
		{
			serializer.serialize(uuid);
			serializer.serialize(path);
			serializer.serialize(name);
			serializer.serialize(sourcePath);
			serializer.serialize(assetPath);
		}

		void deserialize(Serializer& serializer)
		{
			if (serializer.deserialize(uuid) &&
				serializer.deserialize(path) &&
				serializer.deserialize(name) &&
				serializer.deserialize(sourcePath) &&
				serializer.deserialize(assetPath))
			{
			}
		}
	};
}

#endif // !__volucris_asset_path_h__
