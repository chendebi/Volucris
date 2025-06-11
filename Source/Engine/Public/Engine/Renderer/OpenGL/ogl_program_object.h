#ifndef __volucris_ogl_program_object_h__
#define __volucris_ogl_program_object_h__

#include <glad/glad.h>
#include "Engine/Core/types_help.h"
#include <string>
#include <memory>
#include <vector>

namespace volucris
{
	class Uniform;
	class UniformDescription;

	class OGLShaderObject
	{
	public:
		OGLShaderObject(GLenum type, const std::string& source = std::string());

		~OGLShaderObject();

		void setSource(const std::string& source)
		{
			m_source = source;
			dirty();
		}

		bool create();

		bool initialize();

		void release();

		GLenum getType() const { return m_type; }

		uint32 getID() const { return m_id; }

		void dirty() { m_dirty = true; }

		bool isDirty() const { return m_dirty; }

	private:
		uint8 m_dirty;
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

		std::shared_ptr<OGLShaderObject> getAttachShader(GLenum type)
		{
			for (const auto& shader : m_shaders)
			{
				if (shader->getType() == type)
				{
					return shader;
				}
			}
			return nullptr;
		}

		void setAutoReleaseShader(bool enabled)
		{
			m_autoReleaseShader = enabled;
		}

		bool create();

		bool initialize();

		int32 findUniformLocation(const std::string& name);

		int32 findUniformBlockLocation(const std::string& name);

		void release();

		bool valid() const { return m_id > 0; }

		uint32 getID() const { return m_id; }

		void dirty() { m_dirty = true; }

		bool isDirty() const { return m_dirty; }

	protected:
		void autoReleaseShaders();

	private:
		uint8 m_dirty;
		uint8 m_autoReleaseShader;
		uint32 m_id;
		std::vector<std::shared_ptr<OGLShaderObject>> m_shaders;
	};
}

#endif // !__volucris_ogl_program_object_h__
