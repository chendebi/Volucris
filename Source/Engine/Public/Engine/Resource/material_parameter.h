#ifndef __volucris_material_parameter_h__
#define __volucris_material_parameter_h__

#include <glm/glm.hpp>
#include <string>
#include <memory>
#include <Engine/Core/types_help.h>
#include "soft_object_ptr.h"
#include <Engine/Core/material_global.h>
#include <Engine/Core/assert.h>
#include <rttr/registration>

namespace volucris
{
	class Texture2D;
	class UniformValue;

	class MaterialParameter
	{

	public:
		MaterialParameter(const MaterialParameterDescription& description);

		virtual ~MaterialParameter() = default;

		void dirty() { markDirty(true); }

		void markDirty(bool dirty) { m_dirty = dirty; }

		bool isDirty() const { return m_dirty; }

		const MaterialParameterDescription& getDescription() const { return m_description; }

	private:
		uint8 m_dirty;
		MaterialParameterDescription m_description;
	};

	class MaterialParameterFloat : public MaterialParameter
	{
	public:
		MaterialParameterFloat(const MaterialParameterDescription& description)
			: MaterialParameter(description)
			, m_value()
		{
			v_check(description.type == MaterialParameterType::FLOAT)
		}

		void setValue(float value)
		{
			m_value = value;
			dirty();
		}

		float getValue() const { return m_value; }

	public:
		float m_value;
	};

	class MaterialParameterVec3 : public MaterialParameter
	{
	public:
		MaterialParameterVec3(const MaterialParameterDescription& description)
			: MaterialParameter(description)
			, m_value()
		{
			v_check(description.type == MaterialParameterType::VEC3)
		}

		void setValue(const glm::vec3& value)
		{
			m_value = value;
			dirty();
		}

		glm::vec3 getValue() const { return m_value; }


	public:
		glm::vec3 m_value;
	};

	class MaterialParameterTexture2D : public MaterialParameter
	{
	public:
		MaterialParameterTexture2D(const MaterialParameterDescription& description)
			: MaterialParameter(description)
			, m_texture()
		{
			v_check(description.type == MaterialParameterType::TEXTURE2D)
		}

		TSoftObjectPtr<Texture2D> getTexture() const { return m_texture; }

		void setTexture(const TSoftObjectPtr<Texture2D>& texture)
		{
			m_texture = texture;
		}

	public:
		TSoftObjectPtr<Texture2D> m_texture;
	};

}



#endif // !__volucris_material_parameter_h__
