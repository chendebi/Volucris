#include "Renderer/material_proxy.h"
#include "Resource/material.h"
#include "Renderer/OpenGL/ogl_program_object.h"
#include "Renderer/OpenGL/ogl_uniform.h"

namespace volucris
{

	MaterialResourceProxy::MaterialResourceProxy(MaterialResource* resource)
		: m_vss(resource->getVertexShaderSource())
		, m_fss(resource->getFragmentShaderSource())
		, m_program(std::make_unique<OGLProgramObject>())
		, m_descriptions()
	{
		auto vs = std::make_shared<OGLShaderObject>(GL_VERTEX_SHADER);
		vs->setSource(m_vss);
		auto fs = std::make_shared<OGLShaderObject>(GL_FRAGMENT_SHADER);
		fs->setSource(m_fss);
		m_program->attach(vs);
		m_program->attach(fs);
		m_program->setAutoReleaseShader(true);

		for (const auto& desc : resource->getParameterDescriptions())
		{
			m_descriptions.push_back(std::make_shared<UniformDescription>(desc));
		}
		m_program->setUniformDescriptions(m_descriptions);
	}

	MaterialProxy::MaterialProxy(Material* material)
		: m_resource(nullptr)
	{
		m_parameterData = material->getParameterData();
	}

	MaterialProxy::~MaterialProxy()
	{

	}

	void MaterialProxy::setResource(MaterialResourceProxy* resource)
	{
		m_resource = resource;
		m_state.program = resource->m_program.get();

		for (const auto& desc : resource->m_descriptions)
		{
			m_state.uniforms.push_back(std::make_shared<Uniform>(desc, m_parameterData.data()));
		}
	}
}
