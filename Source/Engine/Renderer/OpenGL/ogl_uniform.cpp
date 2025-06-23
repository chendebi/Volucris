#include "Renderer/OpenGL/ogl_uniform.h"
#include <glad/glad.h>
#include <glm/ext.hpp>
#include "Renderer/OpenGL/ogl_program_object.h"
#include <Renderer/OpenGL/ogl_check.h>
#include <Core/material_global.h>

namespace volucris
{
	void UniformUploader::upload(int location, float value)
	{
		glUniform1f(location, value);
	}

	void UniformUploader::upload(int location, int value)
	{
		glUniform1i(location, value);
	}

	void UniformUploader::upload(int location, glm::vec3 value)
	{
		glUniform3fv(location, 1, glm::value_ptr(value));
	}

	void UniformUploader::upload(int location, glm::vec4 value)
	{
		glUniform4fv(location, 1, glm::value_ptr(value));
	}

	void UniformUploader::upload(int location, glm::mat4 value)
	{
		glUniformMatrix4fv(location, 1, false, glm::value_ptr(value));
	}
}
