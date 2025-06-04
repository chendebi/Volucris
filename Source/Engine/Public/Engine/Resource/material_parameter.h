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
		MaterialParameter(Material* material, const MaterialParameterDesc& desc);

		virtual ~MaterialParameter();

		Material* getMaterial() const { return m_material; }

		const MaterialParameterDesc& getDescription() const { return m_desc; }

		void dirty();

	protected:
		Material* m_material;
		MaterialParameterDesc m_desc;
	};

	class MaterialParameterFloat : public MaterialParameter
	{
	public:
		MaterialParameterFloat(Material* material, const MaterialParameterDesc& desc)
			: MaterialParameter(material, desc)
			, m_value()
		{
		}

		void setValue(float value);

	private:
		float m_value;
	};

	class MaterialParameterVec3 : public MaterialParameter
	{
	public:
		MaterialParameterVec3(Material* material, const MaterialParameterDesc& desc)
			: MaterialParameter(material, desc)
			, m_value()
		{
		}

		void setValue(float value);

		void setValue(const glm::vec3& value);

		void setValue(const glm::mat4& value);

	private:
		glm::vec3 m_value;
	};

	class MaterialParameterMat4 : public MaterialParameter
	{
	public:
		MaterialParameterMat4(Material* material, const MaterialParameterDesc& desc)
			: MaterialParameter(material, desc)
			, m_value()
		{ }

		void setValue(const glm::mat4& value)
		{
			m_value = value;
			dirty();
		}

	private:
		glm::mat4 m_value;
	};

	class MaterialParameterTexture2D : public MaterialParameter
	{
	public:
		MaterialParameterTexture2D(Material* material, const MaterialParameterDesc& desc)
			: MaterialParameter(material, desc)
			, m_texture(nullptr)
		{

		}

	private:
		std::shared_ptr<Texture2D> m_texture;
	};
}

#endif // !__volucris_material_parameter_h__
