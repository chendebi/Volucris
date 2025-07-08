#include "RHI/RHIProgram.h"
#include <RHI/RHIOpenGL.h>
#include <RHI/RHIState.h>
#include <RHI/RHICommandList.h>
#include <RHI/RHIShader.h>

namespace volucris
{
	bool RHIProgram::init(RHICommandList* command, const std::vector<std::shared_ptr<RHIShader>>& shaders)
	{
		auto program = getId();
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
		return true;
	}

	uint32 RHIProgram::create(RHIState* state)
	{
		return glCreateProgram();
	}

	void RHIProgram::bind(RHIState* state)
	{
		if (state->program == this)
		{
			return;
		}
		auto id = getId();
		glUseProgram(id);
		state->program = this;
	}

	void RHIProgram::destroy(RHIState* state)
	{
		if (state->program == this)
		{
			state->program = nullptr;
		}
		auto id = getId();
		glDeleteProgram(id);
	}


}
