#ifndef __volucris_ogl_uniform_h__
#define __volucris_ogl_uniform_h__

#include "Engine/Core/types_help.h"
#include <string>
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <Engine/Renderer/material_parameter_description.h>

namespace volucris
{
	class OGLProgramObject;

	class UniformDescription
	{
	public:
		MaterialParameterDesc desc;
		int32 location;

		UniformDescription()
			: desc(), location(-1)
		{ }

		UniformDescription(const MaterialParameterDesc& parameter)
			: desc(parameter), location(-1)
		{ }

		bool valid() const { return location >= 0; }
	};

	class Uniform
	{
	public:
		class Uploader;

	public:
		Uniform(const std::shared_ptr<UniformDescription>& desc, uint8* table);

		~Uniform() = default;

		void setDataTable(uint8* table) { m_table = table; }

		void upload();

		bool valid() const { return m_desc->valid() && m_table; }

		UniformDescription* getDescription() const { return m_desc.get(); }

	protected:
		std::shared_ptr<UniformDescription> m_desc;
		std::shared_ptr<Uploader> m_uploader;
		uint8* m_table;
	};
}

#endif // !__volucris_ogl_uniform_h__
