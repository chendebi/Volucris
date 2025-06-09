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

	MaterialParameter::MaterialParameter(std::string name, MaterialParameterType type)
		: m_dirty(true)
		, m_type(type)
		, m_name(std::move(name))
	{
		
	}

	std::shared_ptr<UniformValue> MaterialParameterFloat::createUniformValue()
	{
		return std::make_shared<UniformValueFloat>(m_value);
	}

	std::shared_ptr<UniformValue> MaterialParameterVec3::createUniformValue()
	{
		return std::make_shared<UniformValueVec3>(m_value);
	}

	std::shared_ptr<UniformValue> MaterialParameterTexture2D::createUniformValue()
	{
		//return std::make_shared<UniformValueVec3>(m_value);
		return nullptr;
	}

}