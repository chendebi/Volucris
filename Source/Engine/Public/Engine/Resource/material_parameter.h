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
	class Texture2D;

	class MaterialParameter
	{
	public:
		enum Type
		{
			UNKNOWN,
			VALUE,
			TEXTURE
		};

		MaterialParameter(Material* material, const MaterialParameterDesc& desc);

		virtual ~MaterialParameter();

		Material* getMaterial() const { return m_material; }

		const MaterialParameterDesc& getDescription() const { return m_desc; }

	protected:
		Type m_type;
		Material* m_material;
		MaterialParameterDesc m_desc;
	};

	class MaterialValueParameter : public MaterialParameter
	{
	public:
		MaterialValueParameter(Material* material, const MaterialParameterDesc& desc, uint8* table);

		void setValue(float value);

		void setValue(const glm::vec3& value);

		void setValue(const glm::mat4& value);

	private:
		uint8* m_dataTable;
	};

	class MaterialTextureParameter : public MaterialParameter
	{
	public:
		MaterialTextureParameter(Material* material, const MaterialParameterDesc& desc, const std::string& guid);

	private:
		std::shared_ptr<Texture2D> m_texture;
	};
}

#endif // !__volucris_material_parameter_h__
