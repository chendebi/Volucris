#ifndef __volucris_material_parameter_h__
#define __volucris_material_parameter_h__

#include <glm/glm.hpp>
#include <string>
#include <memory>
#include <Engine/Core/types_help.h>
#include "soft_object_ptr.h"
#include <Engine/Core/material_global.h>

namespace volucris
{
	class Texture2D;
	class UniformValue;

	class MaterialParameter
	{

	public:
		MaterialParameter(std::string name, MaterialParameterType type);

		virtual ~MaterialParameter() = default;

		void dirty() { markDirty(true); }

		void markDirty(bool dirty) { m_dirty = dirty; }

		bool isDirty() const { return m_dirty; }

		const std::string& getName() const { return m_name; }

		MaterialParameterType getType() const { return m_type; }

		virtual std::shared_ptr<UniformValue> createUniformValue() = 0;

		virtual bool serialize(Serializer& serializer) const = 0;

		virtual void deserialize(Serializer& serializer) = 0;

	private:
		uint8 m_dirty;
		MaterialParameterType m_type;
		std::string m_name;
	};

	class MaterialParameterFloat : public MaterialParameter
	{
	public:
		MaterialParameterFloat(std::string name)
			: MaterialParameter(name, MaterialParameterType::FLOAT)
			, m_value()
		{
		}

		std::shared_ptr<UniformValue> createUniformValue() override;

		void setValue(float value)
		{
			m_value = value;
			dirty();
		}

		bool serialize(Serializer& serializer) const override
		{
			serializer.serialize(m_value);
			return true;
		}

		void deserialize(Serializer& serializer) override
		{
			serializer.deserialize(m_value);
			dirty();
		}

		float getValue() const { return m_value; }

	private:
		float m_value;
	};

	class MaterialParameterVec3 : public MaterialParameter
	{
	public:
		MaterialParameterVec3(std::string name)
			: MaterialParameter(name, MaterialParameterType::VEC3)
			, m_value()
		{
		}

		std::shared_ptr<UniformValue> createUniformValue() override;

		void setValue(const glm::vec3& value)
		{
			m_value = value;
			dirty();
		}

		glm::vec3 getValue() const { return m_value; }

		bool serialize(Serializer& serializer) const override
		{
			serializer.serialize(m_value);
			return true;
		}

		void deserialize(Serializer& serializer) override
		{
			serializer.deserialize(m_value);
			dirty();
		}

	private:
		glm::vec3 m_value;
	};

	class MaterialParameterTexture2D : public MaterialParameter
	{
	public:
		MaterialParameterTexture2D(std::string name, std::string assetPath, int location)
			: MaterialParameter(name, MaterialParameterType::TEXTURE2D)
			, m_location(location)
			, m_texture(std::move(assetPath))
		{

		}

		std::shared_ptr<UniformValue> createUniformValue() override;

		TSoftObjectPtr<Texture2D> getTexture() const { return m_texture; }

		bool serialize(Serializer& serializer) const override
		{
			serializer.serialize(m_texture);
			return true;
		}

		void deserialize(Serializer& serializer) override
		{
			serializer.deserialize(m_texture);
			dirty();
		}

	private:
		int m_location;
		TSoftObjectPtr<Texture2D> m_texture;
	};
}

#endif // !__volucris_material_parameter_h__
