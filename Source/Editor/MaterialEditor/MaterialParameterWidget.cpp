#include "MaterialParameterWidget.h"
#include <imgui/imgui.h>
#include "MaterialTemplate.h"
#include <Engine/Game/MaterialInstance.h>
  
namespace volucris
{
	MaterialParameterWidget::MaterialParameterWidget()
		: Widget()
		, m_material(nullptr)
	{
	}

	void MaterialParameterWidget::setMaterial(const std::shared_ptr<MaterialInstance>& material)
	{
		m_material = material;
		m_parameters.clear();
		for (const auto& parameter : material->getInstanceParameters())
		{
			UniformProperty property;
			std::dynamic_pointer_cast<MaterialTemplate>(material->getMaterial().object())->findProperty(parameter.name, property);
			auto it = m_parameters.find(property.group);
			Parameter param;
			param.desc = parameter;
			param.property = std::move(property);
			if (param.property.displayName.empty())
			{
				param.name = fmt::format("{}: ", parameter.name);
			}
			else
			{
				param.name = fmt::format("{}: ", param.property.displayName);
			}
			if (it == m_parameters.end())
			{
				std::string groupName = param.property.group;
				m_parameters.insert({ std::move(groupName), {std::move(param)}});
			}
			else
			{
				it->second.push_back(std::move(param));
			}
		}
	}

	void MaterialParameterWidget::onBuild(bool)
	{
		ImGui::Begin("ParameterWidget");

		size_t idx = 0;
		for (auto& [group, parameters] : m_parameters)
		{
			if (ImGui::CollapsingHeader(group.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
			{
				for (auto& paramter : parameters)
				{
					auto& info = paramter.desc;
					auto& property = paramter.property;

					ImGui::Text(paramter.name.c_str());
					ImGui::SameLine();

					ImGui::PushID(idx);
					auto type = paramter.desc.type;
					switch (type)
					{
					case volucris::MaterialParamterType::Float:
						if (ImGui::DragFloat("##", std::get_if<float>(&info.value), 0.1f))
						{
							m_material->setFloatParameter(info.name, std::get<float>(info.value));
						}
						break;
					case volucris::MaterialParamterType::Vector4:
						if (ImGui::DragFloat4("##", glm::value_ptr(std::get<glm::vec4>(info.value)), 0.1f))
						{
							m_material->setVector4Parameter(info.name, std::get<glm::vec4>(info.value));
						}
						break;
					case volucris::MaterialParamterType::Mat4:
						break;
					case volucris::MaterialParamterType::Texture2D:
					{
						SoftObject<Texture2D> texture = std::get<SoftObject<Texture2D>>(info.value);
						if (ImGui::BeginCombo("##options", texture.getPath().c_str())) {
							m_textureAssets = AssetManager::getInstance().getAssets<Texture2D>();
							for (auto i = 0; i < m_textureAssets.size(); ++i)
							{
								auto& asset = m_textureAssets[i];
								bool isSelected = texture == asset.path;
								if (ImGui::Selectable(asset.path.c_str(), isSelected))
								{
									texture = SoftObject<Texture2D>(asset.path);
									m_material->setTexture2DParameter(info.name, texture);
								}
								if (isSelected)
								{
									ImGui::SetItemDefaultFocus();
								}
							}
							ImGui::EndCombo();
						}
					}
						break;
					default:
						break;
					}
					ImGui::PopID();
					idx++;
				}
			}
		}
		ImGui::End();
	}
}
