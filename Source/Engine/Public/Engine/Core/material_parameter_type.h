#ifndef __volucris_material_parameter_type_h__
#define __volucris_material_parameter_type_h__

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
	};
}

#endif // !__volucris_material_parameter_type_h__
