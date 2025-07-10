#include "RHI/RHIShader.h"
#include <RHI/RHIOpenGL.h>
#include <Core/Volucris.h>

namespace volucris
{
	RHIShader::RHIShader(ShaderType shaderType)
		: RHIResource()
		, m_shaderType(shaderType)
		, m_valid(false)
		, m_id(0)
	{

	}

	RHIShader::~RHIShader()
	{
		if (m_id > 0)
		{
			glDeleteShader(m_id);
		}
	}

	uint32 RHIShader::getId()
	{
		if (m_id == 0)
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
				v_check(false)
				break;
			}
			m_id = glCreateShader(type);
		}
		return m_id;
	}

	bool RHIShader::init(const std::string& source)
	{
		auto ss = source.c_str();
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
			return false;
		}
		GL_CHECK();
		m_valid = true;
		return true;
	}
}
