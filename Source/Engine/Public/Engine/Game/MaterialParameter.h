#ifndef __volucris_material_parameter_h__
#define __volucris_material_parameter_h__

#include <string>
#include <glm/glm.hpp>
#include <Engine/Render/MaterialParameterInfo.h>
#include <glm/ext.hpp>

namespace volucris
{
	struct MaterialParameter
	{
	public:
		MaterialParameter() : m_name() {}

		MaterialParameter(const std::string& name)
			: m_name(name)
		{

		}

		const std::string& getName() const
		{
			return m_name;
		}

	protected:
		std::string m_name;
	};

	struct MaterialFloatParameter : public MaterialParameter
	{
	public:
		MaterialFloatParameter()
			: MaterialParameter()
			, m_value(0.0)
		{
		}

		MaterialFloatParameter(const std::string& name, float value = 0.0)
			: MaterialParameter(name), m_value(value)
		{
		}

		void setValue(float value) {
			m_value = value;
		}

		float getValue() const {
			return m_value;
		}

		MaterialParameterInfo getParameterInfo() const
		{
			MaterialParameterInfo info;
			info.name = m_name;
			info.type = MaterialParamterType::Float;
			info.value = m_value;
			return info;
		}

	private:
		float m_value;
	};

	struct MaterialVector4Parameter : public MaterialParameter
	{
	public:

		MaterialVector4Parameter()
			: MaterialParameter()
			, m_value(0.0)
		{
		}

		MaterialVector4Parameter(const std::string& name, glm::vec4 value = {0.0,0.0,0.0,1.0})
			: MaterialParameter(name), m_value(value)
		{
		}

		void setValue(const glm::vec4& value) {
			m_value = value;
		}

		const glm::vec4& getValue() const {
			return m_value;
		}

		MaterialParameterInfo getParameterInfo() const
		{
			MaterialParameterInfo info;
			info.name = m_name;
			info.type = MaterialParamterType::Vector4;
			info.value = m_value;
			return info;
		}

	private:
		glm::vec4 m_value;
	};
}

#endif // !__volucris_material_parameter_h__