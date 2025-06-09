#ifndef __volucris_material_parameter_type_h__
#define __volucris_material_parameter_type_h__

#include <string>
#include <Engine/Core/serializer.h>

namespace volucris
{
	enum class MaterialParameterType
	{
		NONE, FLOAT, VEC2, VEC3, VEC4, TEXTURE2D
	};

	struct MaterialParameterDescription
	{
		MaterialParameterType type = MaterialParameterType::NONE;
		std::string name = {};

		void serialize(Serializer& serializer)
		{
			int32 typeInt = static_cast<int32>(type);
			serializer.serialize(typeInt);
			serializer.serialize(name);
		}

		void deserialize(Serializer& serializer)
		{
			int32 typeInt = 0;
			std::string sname;
			if (serializer.deserialize(typeInt) &&
				serializer.deserialize(sname))
			{
				type = static_cast<MaterialParameterType>(typeInt);
				name = std::move(sname);
			}
			else
			{
				type = MaterialParameterType::NONE;
				name.clear();
			}
		}
	};

	constexpr std::string_view MATERIAL_UNIFORM_PRIMITIVE_INFO = "UPrimitiveInfo";
	constexpr std::string_view MATERIAL_UNIFORM_CAMERA_INFO = "UCameraInfo";
	constexpr std::string_view MATERIAL_UNIFORM_DIRECTION_LIGHT = "UDirectionLight";
}

#endif // !__volucris_material_parameter_type_h__
