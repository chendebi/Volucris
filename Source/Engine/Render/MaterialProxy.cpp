#include "Render/MaterialProxy.h"
#include <RHI/RHIProgram.h>
#include <RHI/RHIShader.h>
#include <RHI/RHICommandList.h>
#include <Render/Renderer.h>

namespace volucris
{
	MaterialProxy::MaterialProxy()
		: Object()
		, m_program(nullptr)
	{
	}

	void MaterialProxy::setSource(const std::string& vss, const std::string& fss)
	{
		auto vs = std::make_shared<RHIShader>(RHIShader::VertexShader);
		auto fs = std::make_shared<RHIShader>(RHIShader::FragmentShader);
		if (vs->init(vss) && fs->init(fss))
		{
			m_program = std::make_unique<RHIProgram>();
			m_program->init({ vs, fs });
		}
	}

	void MaterialProxy::setParameters(const std::vector<MaterialParameterInfo>& parameters)
	{
		for (const auto& parameter : parameters)
		{
			auto type = parameter.type;
			switch (type)
			{
			case volucris::MaterialParamterType::Float:
				break;
			case volucris::MaterialParamterType::Vector4:
				break;
			case volucris::MaterialParamterType::Mat4:
				break;
			default:
				break;
			}
		}
	}
}
