#ifndef __volucris_material_parameter_h__
#define __volucris_material_parameter_h__

#include <glm/glm.hpp>
#include <string>
#include <memory>
#include <Engine/Core/types_help.h>
#include <Engine/Renderer/material_parameter_description.h>

namespace volucris
{
	class Material;

	class MaterialParameter
	{
	public:
		MaterialParameter(Material* material, const MaterialParameterDesc& desc, uint8* table);

		~MaterialParameter();

		void setValue(float value);

		void setValue(const glm::vec3& value);

		void setValue(const glm::mat4& value);

		Material* getMaterial() const { return m_material; }

		const MaterialParameterDesc& getDescription() const { return m_desc; }

	private:
		Material* m_material;
		uint8* m_dataTable;
		MaterialParameterDesc m_desc;
	};
}

#endif // !__volucris_material_parameter_h__
