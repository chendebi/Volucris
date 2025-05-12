#include "Resource/material_parameter.h"
#include "Core/assert.h"
#include "Core/volucris.h"

namespace volucris
{
	MaterialParameter::MaterialParameter(Type type)
		: m_type(type)
		, m_name()
	{
	}

	std::unique_ptr<MaterialParameter> MaterialParameter::create(Type type, const std::string& name)
	{
		std::unique_ptr<MaterialParameter> parameter = nullptr;
		switch (type)
		{
		case volucris::MaterialParameter::FLOAT:
			parameter = std::make_unique<MaterialParameterFloat>();
			break;
		case volucris::MaterialParameter::VEC3:
			parameter = std::make_unique<MaterialParameterVec3>();
			break;
		default:
			break;
		}

		if (parameter)
		{
			parameter->m_name = name;
		}

		return parameter;
	}

	void MaterialParameter::setValue(float value)
	{
		if (const auto& parameter = dynamic_cast<MaterialParameterFloat*>(this))
		{
			parameter->setValue(value);
		}
		else
		{
			checkf(false, Engine, "try assign a float value to a material parameter type: {}", (int)m_type)
		}
	}

	void MaterialParameter::setValue(const glm::vec3& value)
	{
		if (const auto& parameter = dynamic_cast<MaterialParameterVec3*>(this))
		{
			parameter->setValue(value);
		}
		else
		{
			checkf(false, Engine, "try assign a vec3 value to a material parameter type: {}", (int)m_type)
		}
	}


}