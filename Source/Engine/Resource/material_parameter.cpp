#include "Resource/material_parameter.h"
#include "Core/assert.h"
#include "Core/volucris.h"
#include <glm/ext.hpp>
#include <Resource/resource_registry.h>
#include <Resource/texture2d.h>

namespace volucris
{

	MaterialParameter::MaterialParameter(Material* material, const MaterialParameterDesc& desc)
		: m_type(UNKNOWN)
		, m_material(material)
		, m_desc(desc)
	{
		if (desc.isTextureValue())
		{
			m_type = TEXTURE;
		}
		else if (desc.isNormalValue())
		{
			m_type = VALUE;
		}
	}

	MaterialParameter::~MaterialParameter()
	{
		//V_LOG_DEBUG(Engine, "destroy material parameter: {}", m_desc.name);
	}

	MaterialValueParameter::MaterialValueParameter(Material* material, const MaterialParameterDesc& desc, uint8* table)
		: MaterialParameter(material, desc)
		, m_dataTable(table)
	{
	}

	void MaterialValueParameter::setValue(float value)
	{
		check(m_desc.type == MaterialParameterDesc::FLOAT && m_dataTable);
		memcpy(m_dataTable, &value, sizeof(float));
	}

	void MaterialValueParameter::setValue(const glm::vec3& value)
	{
		check(m_desc.type == MaterialParameterDesc::VEC3 && m_dataTable);
		memcpy(m_dataTable+m_desc.offset, glm::value_ptr(value), sizeof(glm::vec3));
	}

	void MaterialValueParameter::setValue(const glm::mat4& value)
	{
		//check(m_desc.type == MaterialParameterDesc::MAT4 && m_dataTable);
		memcpy(m_dataTable + m_desc.offset, glm::value_ptr(value), sizeof(glm::mat4));
	}

	MaterialTextureParameter::MaterialTextureParameter(Material* material, const MaterialParameterDesc& desc, const std::string& guid)
		: MaterialParameter(material, desc)
		, m_texture()
	{
		m_texture = ResourceRegistry::Instance().loadResource<Texture2D>(GUID(guid));
	}

}