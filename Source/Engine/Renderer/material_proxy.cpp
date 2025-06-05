#include "Renderer/material_proxy.h"
#include "Resource/material.h"
#include "Resource/material_resource.h"
#include "Renderer/OpenGL/ogl_program_object.h"
#include "Renderer/OpenGL/ogl_uniform.h"
#include <Core/volucris.h>

namespace volucris
{

	MaterialResourceProxy::MaterialResourceProxy()
		: m_vss()
		, m_fss()
		, m_program(std::make_unique<OGLProgramObject>())
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

		std::vector<std::shared_ptr<Uniform>> parameterUniforms;
		for (const auto& name : data.parameterNames)
		{
			parameterUniforms.push_back(std::make_shared<Uniform>(name));
		}

		std::vector<std::shared_ptr<BlockUniform>> blockUniforms;
		if (data.engineParameters & (int32)MaterialInnerParameter::MODEL_MATRIX)
		{
			blockUniforms.push_back(std::make_shared<BlockUniform>("UPrimitiveInfo", MaterialInnerParameter::MODEL_MATRIX));
		}
		if (data.engineParameters & (int32)MaterialInnerParameter::CAMERA_INFO)
		{
			blockUniforms.push_back(std::make_shared<BlockUniform>("UCameraInfo", MaterialInnerParameter::CAMERA_INFO));
		}
		else if (data.engineParameters & (int32)MaterialInnerParameter::CAMERA_INFO)
		{
			blockUniforms.push_back(std::make_shared<BlockUniform>("UDirectionLight", MaterialInnerParameter::DIRECTION_LIGHT));
		}

		m_program->setParameterUniforms(parameterUniforms);
		m_program->setBlockUniforms(blockUniforms);
		m_program->dirty();
	}

	

	MaterialProxy::MaterialProxy()
		: m_resource(nullptr)
		, m_state()
	{

	}

	MaterialProxy::~MaterialProxy()
	{

	}

	void MaterialProxy::updateParameterRenderData(MaterialParameterRenderData renderData)
	{
		const auto& uniforms = m_state.program->getParameterUniforms();
		if (uniforms.size() != renderData.values.size())
		{
			V_LOG_WARN(Engine, "update material render data failed");
			return;
		}

		m_renderData = renderData;
		for (auto idx = 0; idx < uniforms.size(); ++idx)
		{
			m_renderData.values[idx]->setUniform(uniforms[idx]);
		}

		m_state.renderData = &m_renderData;
	}
}
