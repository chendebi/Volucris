#ifndef __volucris_ogl_uniform_h__
#define __volucris_ogl_uniform_h__

#include "Engine/Core/types_help.h"
#include <string>
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <Engine/Core/material_global.h>
#include <Engine/Core/assert.h>

namespace volucris
{
	class OGLProgramObject;

	class Uniform
	{
	public:
		Uniform()
			: m_name()
			, m_location(-1)
		{ }

		Uniform(std::string name)
			: m_name(std::move(name))
			, m_location(-1)
		{

		}

		void setLocation(int location)
		{
			m_location = location;
		}

		const std::string getName() const { return m_name; }

		int getLocation() const { return m_location; }

		void setName(const std::string& name) { m_name = name; }

	private:
		std::string m_name;
		int m_location;
	};

	namespace UniformUploader
	{
		void upload(int location, float value);
		void upload(int location, glm::vec3 value);
		void upload(int location, glm::vec4 value);
		void upload(int location, glm::mat4 value);
	}


	class UniformValue
	{
	public:
		UniformValue(const MaterialParameterDescription& description)
			: m_uniform(nullptr)
			, m_description(description)
		{

		}

		void setUniform(const std::shared_ptr<Uniform>& uniform)
		{
			m_uniform = uniform;
		}

		virtual void updateValue(const std::vector<uint8>& data) = 0;

		const std::shared_ptr<Uniform>& getUniform() const { return m_uniform; }

		virtual ~UniformValue() = default;

		virtual void upload() = 0;

	protected:
		std::shared_ptr<Uniform> m_uniform;
		MaterialParameterDescription m_description;
	};

	class UniformValueFloat : public UniformValue
	{
	public:
		UniformValueFloat(const MaterialParameterDescription& description)
			: UniformValue(description)
			, m_value()
		{
			v_check(description.type == MaterialParameterType::FLOAT);
		}

		void updateValue(const std::vector<uint8>& data) override
		{
			v_check((m_description.offset+ m_description.size) <= data.size())
			memcpy(&m_value, data.data() + m_description.offset, m_description.size);
		}

		void upload() override
		{ 
			if (m_uniform && m_uniform->getLocation() >= 0)
			{
				UniformUploader::upload(m_uniform->getLocation(), m_value);
			}
		}

	private:
		float m_value;
	};

	class UniformValueVec3 : public UniformValue
	{
	public:
		UniformValueVec3(const MaterialParameterDescription& description)
			: UniformValue(description)
			, m_value()
		{
			v_check(description.type == MaterialParameterType::VEC3);
		}

		void updateValue(const std::vector<uint8>& data) override
		{
			v_check((m_description.offset + m_description.size) <= data.size())
				memcpy(&m_value, data.data() + m_description.offset, m_description.size);
		}

		void upload() override
		{
			if (m_uniform && m_uniform->getLocation() >= 0)
			{
				UniformUploader::upload(m_uniform->getLocation(), m_value);
			}
		}

	private:
		glm::vec3 m_value;
	};


	class UniformValueVec4 : public UniformValue
	{
	public:
		UniformValueVec4(const MaterialParameterDescription& description)
			: UniformValue(description)
			, m_value()
		{
			v_check(description.type == MaterialParameterType::VEC4);
		}

		void updateValue(const std::vector<uint8>& data) override
		{
			v_check((m_description.offset + m_description.size) <= data.size())
				memcpy(&m_value, data.data() + m_description.offset, m_description.size);
		}

		void upload() override
		{
			if (m_uniform && m_uniform->getLocation() >= 0)
			{
				UniformUploader::upload(m_uniform->getLocation(), m_value);
			}
		}

	private:
		glm::vec4 m_value;
	};
}

#endif // !__volucris_ogl_uniform_h__
