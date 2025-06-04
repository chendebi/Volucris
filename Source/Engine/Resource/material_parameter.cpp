#include "Resource/material_parameter.h"
#include "Core/assert.h"
#include "Core/volucris.h"
#include <glm/ext.hpp>
#include <Resource/resource_registry.h>
#include <Resource/texture2d.h>
#include <Resource/material.h>

namespace volucris
{

	MaterialParameter::MaterialParameter(Material* material, const MaterialParameterDesc& desc)
		: m_material(material)
		, m_desc(desc)
	{
		
	}

	MaterialParameter::~MaterialParameter()
	{
		//V_LOG_DEBUG(Engine, "destroy material parameter: {}", m_desc.name);
	}

	void MaterialParameter::dirty()
	{
		if (m_material)
		{
			m_material->di
		}
	}
}