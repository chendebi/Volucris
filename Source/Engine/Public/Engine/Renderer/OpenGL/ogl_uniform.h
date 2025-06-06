#ifndef __volucris_ogl_uniform_h__
#define __volucris_ogl_uniform_h__

#include "Engine/Core/types_help.h"
#include <string>
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <Engine/Renderer/material_inner_data.h>

namespace volucris
{
	class OGLProgramObject;

	class Uniform
	{
	public:
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

	private:
		std::string m_name;
		int m_location;
	};

	class BlockUniform : public Uniform
	{
	public:
		BlockUniform(std::string name, MaterialInnerParameter block)
			: Uniform(name), m_block(block)
		{ }

		MaterialInnerParameter getBlockSlot() const { return m_block; }

	private:
		MaterialInnerParameter m_block;
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
		UniformValue()
			: m_uniform(nullptr)
		{

		}

		void setUniform(const std::shared_ptr<Uniform>& uniform)
		{
			m_uniform = uniform;
		}

		const std::shared_ptr<Uniform>& getUniform() const { return m_uniform; }

		virtual ~UniformValue() = default;

		virtual void upload() = 0;
	protected:
		std::shared_ptr<Uniform> m_uniform;
	};

	template <typename T>
	class UniformValueTemplate : public UniformValue
	{
	public:
		UniformValueTemplate()
			: UniformValue()
			, m_value()
		{
		}

		UniformValueTemplate(const T& value)
			: UniformValue()
			, m_value(value)
		{ }

		void setValue(const T& value)
		{
			m_value = value;
		}

		void upload() override
		{
			auto location = m_uniform->getLocation();
			if (location >= 0)
			{
				UniformUploader::upload(location, m_value);
			}
		}

	private:
		T m_value;
	};

	using UniformValueFloat = UniformValueTemplate<float>;
	using UniformValueVec3 = UniformValueTemplate<glm::vec3>;
	//using UniformValueMat4 = UniformValueTemplate<glm::mat4>;
}

#endif // !__volucris_ogl_uniform_h__
