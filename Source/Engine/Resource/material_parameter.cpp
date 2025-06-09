#include "Resource/material_parameter.h"
#include "Core/assert.h"
#include "Core/volucris.h"
#include <glm/ext.hpp>
#include <Resource/resource_registry.h>
#include <Resource/texture2d.h>
#include <Resource/material.h>
#include <Renderer/OpenGL/ogl_uniform.h>

namespace volucris
{

	MaterialParameter::MaterialParameter(const MaterialParameterDescription& description)
		: m_dirty(true)
		, m_description(description)
	{
		
	}

}