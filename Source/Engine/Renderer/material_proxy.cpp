#include "Renderer/material_proxy.h"
#include "Resource/material.h"
#include "Resource/material_resource.h"
#include "Renderer/OpenGL/ogl_program_object.h"
#include "Renderer/OpenGL/ogl_uniform.h"
#include <Core/volucris.h>
#include <Core/material_global.h>
#include <Renderer/texture2d_proxy.h>

namespace volucris
{

	MaterialResourceProxy::MaterialResourceProxy()
		: m_dirty(true)
		, m_vss()
		, m_fss()
		, m_program(std::make_unique<OGLProgramObject>())
		, m_cameraInfoUniformBlock(std::make_unique<Uniform>())
		, m_primitiveInfoUniformBlock(std::make_unique<Uniform>())
		, m_directLightUniformBlock(std::make_unique<Uniform>())
	{
		auto vs = std::make_shared<OGLShaderObject>(GL_VERTEX_SHADER);
		auto fs = std::make_shared<OGLShaderObject>(GL_FRAGMENT_SHADER);
		m_program->attach(vs);
		m_program->attach(fs);
		m_program->setAutoReleaseShader(true);
	}

	void MaterialResourceProxy::update(const MaterialRenderData& data)
	{
		m_program->getAttachShader(GL_VERTEX_SHADER)->setSource(data.vss);
		m_program->getAttachShader(GL_FRAGMENT_SHADER)->setSource(data.fss);
		m_descriptions = data.descriptions;
		for (const auto& description : m_descriptions)
		{
			m_uniforms.push_back(std::make_shared<Uniform>(description.name));
		}
		m_dirty = true;
		m_program->dirty();
	}

	std::vector<std::unique_ptr<UniformValue>> MaterialResourceProxy::createUniformValues() const
	{
		std::vector<std::unique_ptr<UniformValue>> values;
		for (auto idx = 0; idx < m_descriptions.size(); ++idx)
		{
			std::unique_ptr<UniformValue> value = nullptr;
			auto description = m_descriptions[idx];
			MaterialParameterType type = description.type;
			switch (type)
			{
			case volucris::MaterialParameterType::NONE:
				break;
			case volucris::MaterialParameterType::FLOAT:
				value = std::make_unique<UniformValueFloat>(description);
				break;
			case volucris::MaterialParameterType::VEC2:
				break;
			case volucris::MaterialParameterType::VEC3:
				value = std::make_unique<UniformValueVec3>(description);
				break;
			case volucris::MaterialParameterType::VEC4:
				value = std::make_unique<UniformValueVec4>(description);
				break;
			case volucris::MaterialParameterType::TEXTURE2D:
				break;
			default:
				break;
			}
			value->setUniform(m_uniforms[idx]);
			values.emplace_back(std::move(value));
		}
		return values;
	}

	bool MaterialResourceProxy::ready()
	{
		if (!m_dirty)
		{
			return true;
		}

		if (!m_program->create() || !m_program->initialize())
		{
			return false;
		}

		for (const auto& uniform : m_uniforms)
		{
			uniform->setLocation(m_program->findUniformLocation(uniform->getName()));
		}

		m_cameraInfoUniformBlock->setLocation(m_program->findUniformBlockLocation("UCameraInfo"));
		m_primitiveInfoUniformBlock->setLocation(m_program->findUniformBlockLocation("UPrimitiveInfo"));
		m_directLightUniformBlock->setLocation(m_program->findUniformBlockLocation("UDirectionLight"));

		if (m_directLightUniformBlock->getLocation() >= 0)
		{
			glUniformBlockBinding(m_program->getID(), m_directLightUniformBlock->getLocation(), 0);
		}

		if (m_cameraInfoUniformBlock->getLocation() >= 0)
		{
			glUniformBlockBinding(m_program->getID(), m_cameraInfoUniformBlock->getLocation(), 1);
		}

		if (m_primitiveInfoUniformBlock->getLocation() >= 0)
		{
			glUniformBlockBinding(m_program->getID(), m_primitiveInfoUniformBlock->getLocation(), 2);
		}

		m_dirty = false;
		return true;
	}

	

	MaterialProxy::MaterialProxy()
		: m_initialized(false)
		, m_dirty(true)
		, m_resource(nullptr)
		, m_state()
	{

	}

	MaterialProxy::~MaterialProxy()
	{

	}

	void MaterialProxy::updateParameterRenderData(std::vector<uint8> buffer, std::vector<std::shared_ptr<Texture2DProxy>> textures)
	{
		m_bufferData = std::move(buffer);
		m_textures = std::move(textures);
		m_dirty = true;
	}

	void MaterialProxy::update()
	{
		if (!m_initialized)
		{
			m_uniformValues = m_resource->createUniformValues();
			m_state.material = m_resource.get();
			m_state.uniforms.clear();
			m_state.textures.clear();
			m_state.uniforms.reserve(m_uniformValues.size());
			m_state.textures.reserve(m_textures.size());
			for (auto& value : m_uniformValues)
			{
				m_state.uniforms.push_back(value.get());
			}

			for (const auto& texture : m_textures)
			{
				m_state.textures.push_back(texture->getTexture2DObject());
			}

			m_initialized = true;
		}

		if (m_dirty)
		{
			for (const auto& value : m_uniformValues)
			{
				value->updateValue(m_bufferData);
			}
			m_dirty = false;
		}
	}
}
