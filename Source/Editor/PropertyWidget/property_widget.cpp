#include "property_widget.h"
#include <rttr/type.h>
#include <rttr/property.h>
#include <EditorEntry/editor_core.h>
#include <Engine/Scene/actor.h>
#include <imgui.h>
#include <fmt/format.h>
#include <glm/gtc/type_ptr.hpp>

namespace volucris
{
	PropertyWidget::PropertyWidget()
		: Widget()
        , m_object(nullptr)
        , value({0,0,0})
	{

	}

	void PropertyWidget::build()
	{
        ImGui::Begin("Properties");
        
        if (m_object)
        {
            ImGui::SeparatorText(m_object->getDisplayName().c_str());
        }

        for (auto& group : m_groups)
        {
            if (ImGui::CollapsingHeader(group.name.c_str()))
            {
                for (auto& property : group.vec3Properties)
                {
                    ImGui::Text(fmt::format("{}: ", property.name).c_str());
                    ImGui::SameLine();
                    ImGui::DragFloat3(fmt::format("##{}", property.name).c_str(), glm::value_ptr(property.value), 0.1f);
                    if (ImGui::IsItemDeactivatedAfterEdit()) {
                        property.property.set_value(m_object, property.value);
                    }
                }
            }
        }

        ImGui::End();
	}

    void PropertyWidget::setSceneObject(SceneObject* object)
    {
        m_object = object;
        rttr::type obj_type = rttr::type::get(*object);

        m_groups.clear();
        
        for (auto prop : obj_type.get_properties()) {
            auto groupName = prop.get_metadata("Group").to_string();
            PropertyGroup* propertyGroup = nullptr;
            for (auto& group : m_groups)
            {
                if (group.name == groupName)
                {
                    propertyGroup = &group;
                }
            }

            if (!propertyGroup)
            {
                PropertyGroup group;
                group.name = groupName;
                m_groups.push_back(group);
                propertyGroup = &(*m_groups.rbegin());
            }

            auto value = prop.get_value(object);
            if (value.is_type<glm::vec3>())
            {
                Vector3Property property = Vector3Property(prop);
                property.name = std::string(prop.get_name());
                property.value = value.get_value<glm::vec3>();
                propertyGroup->vec3Properties.emplace_back(std::move(property));
            }
        }
	}
}

