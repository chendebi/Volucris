#include "RHI/RHIUniform.h"
#include <RHI/RHIOpenGL.h>
#include <RHI/RHIProgram.h>
#include <glm/ext.hpp>

namespace volucris
{
	bool RHIUniform::init(RHIProgram* program, const std::string& name)
	{
		m_name = name;
		m_location = glGetUniformLocation(program->getId(), name.c_str());
		return m_location >= 0;
	}

	void RHIUniformFloat::setValue(float value)
	{
		glUniform1f(m_location, value);
	}

	void RHIUniformVec4::setValue(const glm::vec4& value)
	{
		glUniform4fv(m_location, 1, glm::value_ptr(value));
	}

	void RHIUniformMat4::setValue(const glm::mat4& value)
	{
		glUniformMatrix4fv(m_location, 1, false, glm::value_ptr(value));
	}
}
