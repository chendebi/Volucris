#include "RHI/RHIProgram.h"
#include <RHI/RHIOpenGL.h>
#include <RHI/RHIState.h>
#include <RHI/RHICommandList.h>
#include <RHI/RHIShader.h>

namespace volucris
{
	RHIProgram::RHIProgram()
		: RHIResource()
		, m_id(0)
		, m_valid(false)
	{
	}

	RHIProgram::~RHIProgram()
	{
		if (m_id > 0)
		{
			glDeleteProgram(m_id);
		}
	}

	uint32 RHIProgram::getId()
	{
		if (m_id == 0)
		{
			m_id = glCreateProgram();
		}
		return m_id;
	}

	bool RHIProgram::init(const std::vector<std::shared_ptr<RHIShader>>& shaders)
	{
		auto program = m_id;
		for (const auto& shader : shaders)
		{
			glAttachShader(program, shader->getId());
		}
		glLinkProgram(program);
		int success = 0;
		glGetProgramiv(program, GL_LINK_STATUS, &success);
		if (!success)
		{
			char msg[512];
			glGetProgramInfoLog(program, 512, nullptr, msg);
			V_LOG_WARN(Engine, "link program failed.");
			V_LOG_WARN(Engine, "{}", msg);
			return false;
		}
		m_valid = true;
		return true;
	}
}
