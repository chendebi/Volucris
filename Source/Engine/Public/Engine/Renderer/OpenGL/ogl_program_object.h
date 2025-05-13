#ifndef __volucris_ogl_program_object_h__
#define __volucris_ogl_program_object_h__

#include <glad/glad.h>
#include "Engine/Core/types_help.h"
#include <string>
#include <memory>
#include <vector>

namespace volucris
{
	class OGLShaderObject
	{
	public:
		OGLShaderObject(GLenum type, const std::string& source = std::string());

		~OGLShaderObject();

		void setSource(const std::string& source)
		{
			m_source = source;
		}

		bool initialize();

		void release();

		GLenum getType() const { return m_type; }

		uint32 getID() const { return m_id; }

	private:
		GLenum m_type;
		uint32 m_id;
		std::string m_source;
	};

	class OGLProgramObject
	{
	public:
		OGLProgramObject();

		~OGLProgramObject();

		void attach(const std::shared_ptr<OGLShaderObject>& shader)
		{
			m_shaders.push_back(shader);
		}

		bool initialize();

		void release();

	private:
		uint32 m_id;
		std::vector<std::shared_ptr<OGLShaderObject>> m_shaders;
	};
}

#endif // !__volucris_ogl_program_object_h__
