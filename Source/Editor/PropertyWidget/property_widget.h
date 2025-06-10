#ifndef __volucris_property_widget_h__
#define __volucris_property_widget_h__

#include <vector>
#include <string>
#include <Engine/Application/widget.h>
#include <glm/glm.hpp>
#include <rttr/property.h>

namespace volucris
{
	class SceneObject;

	struct Vector3Property
	{
		std::string name;
		glm::vec3 value;
		const rttr::property property;

		Vector3Property(const rttr::property& prop)
			: name(), value(), property(prop)
		{ }
	};

	struct FloatProperty
	{
		std::string name;
		float value;
	};

	struct PropertyGroup
	{
		std::string name;
		std::vector<Vector3Property> vec3Properties;
		std::vector<FloatProperty> floatProperties;
	};

	class PropertyWidget : public Widget
	{
	public:
		PropertyWidget();

		void build() override;

		void setSceneObject(SceneObject* object);

	private:
		glm::vec3 value;
		SceneObject* m_object;
		std::vector<PropertyGroup> m_groups;
	};
}

#endif // !__volucris_property_widget_h__
