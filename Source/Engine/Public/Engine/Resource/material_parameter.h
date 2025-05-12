#ifndef __volucris_material_parameter_h__
#define __volucris_material_parameter_h__

#include <glm/glm.hpp>
#include <string>
#include <memory>

namespace volucris
{
	class MaterialParameter
	{
	public:
		enum Type
		{
			UNKNOWN,
			FLOAT,
			VEC3
		};

	public:
		MaterialParameter(Type type);

		virtual ~MaterialParameter() = default;

		static std::unique_ptr<MaterialParameter> create(Type type, const std::string& name);

		void setValue(float value);

		void setValue(const glm::vec3& value);

	private:
		Type m_type;
		std::string m_name;
	};

	template<typename T>
	class MaterialParameterTemplate : public MaterialParameter
	{
		T m_value;
	public:
		MaterialParameterTemplate() : MaterialParameter(UNKNOWN), m_value() {}
	};

	template<>
	MaterialParameterTemplate<float>::MaterialParameterTemplate()
		: MaterialParameter(FLOAT), m_value(0.f) { }

	template<>
	MaterialParameterTemplate<glm::vec3>::MaterialParameterTemplate()
		: MaterialParameter(VEC3), m_value() {}

	using MaterialParameterFloat = MaterialParameterTemplate<float>;
	using MaterialParameterVec3 = MaterialParameterTemplate<glm::vec3>;
}

#endif // !__volucris_material_parameter_h__
