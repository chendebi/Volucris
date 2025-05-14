#ifndef __volucris_material_parameter_description_h__
#define __volucris_material_parameter_description_h__

#include "Engine/Core/types_help.h"
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
			VEC3
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
			default:
				break;
			}
			return 0;
		}

		Type type = UNKNOWN;
		std::string name = std::string();
		size_t offset = 0;
	};
}

#endif // !__volucris_material_parameter_description_h__
