#include "Resource/material_parameter.h"
#include "Core/assert.h"
#include "Core/volucris.h"
#include <glm/ext.hpp>

namespace volucris
{

	MaterialParameter::MaterialParameter(Material* material, const MaterialParameterDesc& desc, uint8* table)
		: m_material(material)
		, m_desc(desc)
		, m_dataTable(table)
	{

	}

	MaterialParameter::~MaterialParameter()
	{
		//V_LOG_DEBUG(Engine, "destroy material parameter: {}", m_desc.name);
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

	void MaterialParameter::setValue(const glm::mat4& value)
	{
		//check(m_desc.type == MaterialParameterDesc::MAT4 && m_dataTable);
		memcpy(m_dataTable + m_desc.offset, glm::value_ptr(value), sizeof(glm::mat4));
	}

}