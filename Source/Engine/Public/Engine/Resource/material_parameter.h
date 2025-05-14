#ifndef __volucris_material_parameter_h__
#define __volucris_material_parameter_h__

#include <glm/glm.hpp>
#include <string>
#include <memory>
#include <Engine/Core/types_help.h>
#include <Engine/Renderer/material_parameter_description.h>

namespace volucris
{
	class MaterialParameter
	{
	public:
		MaterialParameter(const MaterialParameterDesc& desc, uint8* table);

		~MaterialParameter() = default;

		void setValue(float value);

		void setValue(const glm::vec3& value);

		const MaterialParameterDesc& getDescription() const { return m_desc; }

	private:
		uint8* m_dataTable;
		MaterialParameterDesc m_desc;
	};
}

#endif // !__volucris_material_parameter_h__
