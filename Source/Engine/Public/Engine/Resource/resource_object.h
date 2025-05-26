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
	inline bool getJsonValueString(const rapidjson::Value& object, const char* key, std::string& value)
	{
		auto it = object.FindMember(key);
		if (it != object.MemberEnd() && it->value.IsString())
		{
			value = it->value.GetString();
			return true;
		}
		return false;
	}

	inline bool getJsonValueStringList(const rapidjson::Value& object, const char* key, std::vector<std::string>& value)
	{
		auto it = object.FindMember(key);
		if (it != object.MemberEnd() && it->value.IsArray())
		{
			std::vector<std::string> values;
			auto array = it->value.GetArray();
			for (auto idx = 0; idx < array.Size(); ++idx)
			{
				if (array[idx].IsString())
				{
					values.push_back(array[idx].GetString());
				}
			}
			value = std::move(values);
			return true;
		}
		return false;
	}

	inline bool getJsonValueInt(const rapidjson::Value& object, const char* key, int& value)
	{
		auto it = object.FindMember(key);
		if (it != object.MemberEnd() && it->value.IsInt())
		{
			value = it->value.GetInt();
			return true;
		}
		return false;
	}

	inline bool getJsonValueObject(const rapidjson::Value& object, rapidjson::Document::AllocatorType& allocator, const char* key, rapidjson::Value& value)
	{
		auto it = object.FindMember(key);
		if (it != object.MemberEnd() && it->value.IsObject())
		{
			value.CopyFrom(it->value, allocator);
			return true;
		}
		return false;
	}

	class ResourceObject
	{
	public:
		ResourceObject() = default;

		virtual ~ResourceObject() = default;

		const ResourcePath& getResourcePath() const { return m_path; }

		const std::string getResourceFullPath() const { return m_path.fullpath; }

		void setResourcePath(const ResourcePath& path) { m_path = path; }

		const ResourceMeta& getMetaData() const { return m_metaData; }

	protected:
		friend class ResourceRegistry;
		virtual bool serialize(Serializer& serializer) const { return false; }
		
		virtual void deserialize(Serializer& serializer) { }

	private:
		friend class ResourceManager;
		ResourcePath m_path;
		ResourceMeta m_metaData;
	};
}

#endif // !__volucris_resource_object_h__
