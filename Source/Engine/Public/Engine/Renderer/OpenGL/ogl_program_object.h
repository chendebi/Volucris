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
		}

		bool create();

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

		void setAutoReleaseShader(bool enabled)
		{
			m_autoReleaseShader = enabled;
		}

		bool initialize();

		void release();

		bool valid() const { return m_id > 0; }

		uint32 getID() const { return m_id; }

		void setUniformDescriptions(
			const std::vector<std::shared_ptr<UniformDescription>>& descritions,
			const std::vector<UniformDescription>& uniformBlockDescriptions);

		const std::vector<UniformDescription>& getUniformBlockDescriptions() const
		{
			return m_uniformBlockDescriptions;
		}

	protected:
		void autoReleaseShaders();

	private:
		uint32 m_id;
		uint8 m_autoReleaseShader;
		std::vector<std::shared_ptr<OGLShaderObject>> m_shaders;
		std::vector<std::shared_ptr<UniformDescription>> m_uniformDescriptions;
		std::vector<UniformDescription> m_uniformBlockDescriptions;
	};
}

#endif // !__volucris_ogl_program_object_h__
