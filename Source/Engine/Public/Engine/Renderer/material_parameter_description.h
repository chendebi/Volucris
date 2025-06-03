#ifndef __volucris_material_parameter_description_h__
#define __volucris_material_parameter_description_h__

#include "Engine/Core/types_help.h"
#include <Engine/Core/serializer.h>
#include <vector>
#include <glm/glm.hpp>
#include <string>

namespace volucris
{
	struct MaterialParameterDesc
	{
		enum Type
		{
			UNKNOWN,
			FLOAT,
			VEC3,
			MAT4,
			MODEL_INFO,
			CAMERA_INFO,
			DIRECTION_LIGHT,
			TEXTURE2D
		};

		static size_t sizeOfType(Type type)
		{
			switch (type)
			{
			case UNKNOWN:
				break;
			case FLOAT:
				return sizeof(float);
			case VEC3:
				return sizeof(glm::vec3);
			case MAT4:
			case MODEL_INFO:
				return sizeof(glm::mat4);
			default:
				break;
			}
			return 0;
		}

		bool isNormalValue() const
		{
			return type != TEXTURE2D && !isUniformBlockBinding();
		}

		bool isTextureValue() const
		{
			return type == TEXTURE2D;
		}

		bool isUniformBlockBinding() const
		{
			return type == CAMERA_INFO || type == DIRECTION_LIGHT;
		}

		Type type = UNKNOWN;
		std::string name = std::string();
		size_t offset = 0;

		void serialize(Serializer& serializer)
		{
			serializer.serialize((int)type);
			serializer.serialize(name);
		}

		void deserialize(Serializer& serializer)
		{
			int typeValue;
			if (serializer.deserialize(typeValue) &&
				serializer.deserialize(name))
			{
				type = (Type)typeValue;
			}
		};
	};
}

#endif // !__volucris_material_parameter_description_h__
