#include "Resource/material_parameter.h"
#include "Core/assert.h"
#include "Core/volucris.h"
#include <glm/ext.hpp>

namespace volucris
{

	MaterialParameter::MaterialParameter(const MaterialParameterDesc& desc, uint8* table)
		: m_desc(desc)
		, m_dataTable(table)
	{
	}

	void MaterialParameter::setValue(float value)
	{
		check(m_desc.type == MaterialParameterDesc::FLOAT && m_dataTable);
		memcpy(m_dataTable, &value, sizeof(float));
	}

	void MaterialParameter::setValue(const glm::vec3& value)
	{
		check(m_desc.type == MaterialParameterDesc::VEC3 && m_dataTable);
		memcpy(m_dataTable+m_desc.offset, glm::value_ptr(value), sizeof(glm::vec3));
	}


}