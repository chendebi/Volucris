#include "RHI/RHIShader.h"
#include <RHI/RHIOpenGL.h>
#include <Core/Volucris.h>

namespace volucris
{
	RHIShader::RHIShader(ShaderType shaderType, const std::string& source)
		: RHIResource()
		, m_shaderType(shaderType)
		, m_source(source)
	{

	}

	bool RHIShader::init(RHICommandList* command)
	{
		auto ss = m_source.c_str();
		auto shader = getId();
		glShaderSource(shader, 1, &ss, nullptr);
		glCompileShader(shader);

		int successs = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &successs);
		if (!successs)
		{
			char msg[512];
			glGetShaderInfoLog(shader, 512, nullptr, msg);
			V_LOG_WARN(Engine, "shader compile failed");
			V_LOG_WARN(Engine, "{}", msg);
		}
		GL_CHECK()
		return successs > 0;
	}

	uint32 RHIShader::create(RHIState* state)
	{
		GLenum type = GL_NONE;
		switch (m_shaderType)
		{
		case volucris::RHIShader::VertexShader:
			type = GL_VERTEX_SHADER;
			break;
		case volucris::RHIShader::FragmentShader:
			type = GL_FRAGMENT_SHADER;
			break;
		default:
			break;
		}
		return glCreateShader(type);
	}

	void RHIShader::destroy(RHIState* state)
	{
		auto id = getId();
		if (id > 0)
		{
			glDeleteShader(id);
		}
	}
}
