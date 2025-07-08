#ifndef __volucris_rhi_uniform_h__
#define __volucris_rhi_uniform_h__

#include <string>
#include <Engine/Core/TypesHelp.h>
#include <glm/glm.hpp>

namespace volucris
{
	class RHIProgram;

	class RHIUniform
	{
	public:
		RHIUniform() : m_name(), m_location(-1) {}

		bool init(RHIProgram* program, const std::string& name);

		const std::string& getName() const { return m_name; }

	protected:
		std::string m_name;
		int32 m_location;
	};

	class RHIUniformFloat : public RHIUniform
	{
	public:
		RHIUniformFloat() : RHIUniform() {}

		void setValue(float value);
	};

	class RHIUniformVec4 : public RHIUniform
	{
	public:
		RHIUniformVec4() : RHIUniform() {}

		void setValue(const glm::vec4& value);
	};

	class RHIUniformMat4 : public RHIUniform
	{
	public:
		RHIUniformMat4() : RHIUniform() {}

		void setValue(const glm::mat4& value);
	};
}

#endif // !__volucris_rhi_uniform_h__
