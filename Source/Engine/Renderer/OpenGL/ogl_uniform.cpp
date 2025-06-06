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

	BlockUniform::BlockUniform(MaterialUniformBlock block)
		: Uniform(), m_block(block)
	{
		switch (block)
		{
		case volucris::PRIMITIVE_INFO:
			setName(std::string(MATERIAL_UNIFORM_PRIMITIVE_INFO));
			break;
		case volucris::CAMERA_INFO:
			setName(std::string(MATERIAL_UNIFORM_CAMERA_INFO));
			break;
		case volucris::DIRECTION_LIGHT:
			setName(std::string(MATERIAL_UNIFORM_DIRECTION_LIGHT));
			break;
		default:
			V_LOG_WARN(Engine, "unsupport uniform block slot: {}", (int)block);
			break;
		}
	}
}
