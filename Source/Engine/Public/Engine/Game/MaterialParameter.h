#ifndef __volucris_material_parameter_h__
#define __volucris_material_parameter_h__

#include <string>
#include <glm/glm.hpp>
#include <Engine/Render/MaterialParameterInfo.h>
#include <glm/ext.hpp>
#include <Engine/Core/GlmHelp.h>

namespace volucris
{
	struct MaterialFloatParameter
	{
	public:
		MaterialFloatParameter()
			: m_name()
			, m_value(0.0)
		{
		}

		MaterialFloatParameter(const std::string& name, float value = 0.0)
			: m_name(name), m_value(value)
		{
		}

		void setValue(float value) {
			m_value = value;
		}

		float getValue() const {
			return m_value;
		}

		const std::string& getName() const { return m_name; }

		template <class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& m_name;
			ar& m_value;
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
		std::string m_name;
		float m_value;
	};

	struct MaterialVector4Parameter
	{
	public:

		MaterialVector4Parameter()
			: m_name()
			, m_value(0.0)
		{
		}

		MaterialVector4Parameter(const std::string& name, glm::vec4 value = {0.0,0.0,0.0,1.0})
			: m_name(name), m_value(value)
		{
		}

		void setValue(const glm::vec4& value) {
			m_value = value;
		}

		const std::string& getName() const { return m_name; }

		const glm::vec4& getValue() const {
			return m_value;
		}

		template <class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& m_name;
			ar& m_value;
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
		std::string m_name;
		glm::vec4 m_value;
	};
}

#endif // !__volucris_material_parameter_h__