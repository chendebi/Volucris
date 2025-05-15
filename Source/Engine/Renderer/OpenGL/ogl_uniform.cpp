#include "Renderer/OpenGL/ogl_uniform.h"
#include <glad/glad.h>
#include <glm/ext.hpp>
#include "Renderer/OpenGL/ogl_program_object.h"
#include <Renderer/OpenGL/ogl_check.h>

namespace volucris
{
	class Uniform::Uploader
	{
	public:
		virtual ~Uploader() = default;
		virtual void upload(int32 location, uint8* addr) = 0;
	};

	class FloatUploader : public Uniform::Uploader
	{
	public:
		void upload(int32 location, uint8* addr) override
		{
			glUniform1f(location, *reinterpret_cast<float*>(addr));
		}
	};

	class Vec3Uploader : public Uniform::Uploader
	{
	public:
		void upload(int32 location, uint8* addr) override
		{
			glUniform3fv(location, 1, reinterpret_cast<float*>(addr));
		}
	};

	class Mat4Uploader : public Uniform::Uploader
	{
	public:
		void upload(int32 location, uint8* addr) override
		{
			glUniformMatrix4fv(location, 1, false, reinterpret_cast<float*>(addr));
		}
	};

	Uniform::Uniform(const std::shared_ptr<UniformDescription>& desc, uint8* table)
		: m_desc(desc)
		, m_uploader(nullptr)
		, m_table(table)
	{
		switch (desc->desc.type)
		{
		case MaterialParameterDesc::FLOAT:
			m_uploader = std::make_shared<FloatUploader>();
			break;
		case MaterialParameterDesc::VEC3:
			m_uploader = std::make_shared<Vec3Uploader>();
			break;
		case MaterialParameterDesc::MAT4:
			m_uploader = std::make_shared<Mat4Uploader>();
			break;
		default:
			break;
		}
	}

	void Uniform::upload()
	{
		if (valid())
		{
			m_uploader->upload(m_desc->location, m_table + m_desc->desc.offset);
		}
	}

}
