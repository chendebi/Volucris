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
                ImGui::Columns(2); // 2列
                ImGui::SetColumnWidth(0, 100);
                for (auto& property : group.stringProperties)
                {
                    ImGui::Text(fmt::format("{}: ", property.name).c_str());
                    ImGui::NextColumn();
                    float available_width = ImGui::GetContentRegionAvail().x;
                    ImGui::PushItemWidth(available_width);
                    ImGui::Text(property.value.c_str());
                    if (ImGui::IsItemDeactivatedAfterEdit()) {
                        property.property.set_value(m_object, property.value);
                    }
                    ImGui::PopItemWidth();
                    ImGui::NextColumn();
                }
                for (auto& property : group.vec3Properties)
                {
                    ImGui::Text(fmt::format("{}: ", property.name).c_str());
                    //ImGui::SameLine();
                    ImGui::NextColumn();
                    float available_width = ImGui::GetContentRegionAvail().x;
                    ImGui::PushItemWidth(available_width);
                    ImGui::DragFloat3(fmt::format("##{}", property.name).c_str(), glm::value_ptr(property.value), 0.1f);
                    if (ImGui::IsItemDeactivatedAfterEdit()) {
                        property.property.set_value(m_object, property.value);
                    }
                    ImGui::PopItemWidth();
                    ImGui::NextColumn();
                }
                ImGui::Columns(1); // 2列
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
            else if (value.is_type<float>())
            {
                FloatProperty property = FloatProperty(prop);
                property.name = std::string(prop.get_name());
                property.value = value.get_value<float>();
                propertyGroup->floatProperties.emplace_back(std::move(property));
            }
            else if (value.is_type<std::string>())
            {
                StringProperty property = StringProperty(prop);
                property.name = std::string(prop.get_name());
                property.value = value.get_value<std::string>();
                propertyGroup->stringProperties.emplace_back(std::move(property));
            }
        }
	}
}

