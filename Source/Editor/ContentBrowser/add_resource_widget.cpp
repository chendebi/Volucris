#include "add_resource_widget.h"
#include <imgui.h>
#include <Engine/Resource/resource_registry.h>
#include <Engine/Resource/material.h>
#include <Engine/Resource/material_resource.h>
#include <Engine/Resource/material.h>
#include "path_tree_widget.h"
#include "EditorEntry/editor_core.h"
#include <iostream>

namespace volucris
{
	static const char* ResourceTypes[] = { "Material", "StaticMesh", "Texture(Unsupport)" };

	AddResourceDialog::AddResourceDialog(PathTreeWidget* pathWidget)
		: Widget()
		, m_opened(false)
		, m_showHint(false)
		, m_currentIdx(0)
		, m_buffer()
		, m_pathWidget(pathWidget)
	{
		m_buffer.resize(64);
	}

	void AddResourceDialog::build()
	{
		if (m_opened)
		{
			ImGui::OpenPopup("add resource");
		}

		if (ImGui::BeginPopupModal("add resource", &m_opened, ImGuiWindowFlags_AlwaysAutoResize)) {
			ImGui::Text("Resource       :");
			ImGui::SameLine(); // 保持在同一行
			ImGui::Combo("##resource_type", &m_currentIdx, ResourceTypes, IM_ARRAYSIZE(ResourceTypes));

			ImGui::Text("AssetName      :");
			ImGui::SameLine();
			ImGui::InputTextWithHint("##asset_name", "Name", m_buffer.data(), m_buffer.size());

			if (m_currentIdx == 0)
			{
				buildMaterialInput();
			}

			if (m_showHint)
			{
				ImGui::Text("resource is invalid.");
			}

			{
				// 获取窗口内容区域宽度
				float window_width = ImGui::GetContentRegionAvail().x;

				// 右对齐按钮
				float button_width = 120.0f;
				ImGui::SetCursorPosX(window_width - button_width);
				if (ImGui::Button("Ok", ImVec2(button_width, 0)))
				{
					m_showHint = !addResource();
					if (!m_showHint)
					{
						m_opened = false;
					}
				}

				ImGui::SameLine();
				ImGui::SetCursorPosX(window_width - button_width - button_width - 6);
				if (ImGui::Button("Cancel", ImVec2(button_width, 0)))
				{
					m_opened = false;
				}
			}

			ImGui::EndPopup();
		}
	}

	bool AddResourceDialog::addResource()
	{
		if (m_currentIdx == 0)
		{
			return addMaterialResource();
		}
		return false;
	}

	bool AddResourceDialog::addMaterialResource()
	{
		std::string vs, fs;
		if (ResourcePath::SystemPathToResourcePath(m_materialInfo.vsPath.data(), vs)
			&& ResourcePath::SystemPathToResourcePath(m_materialInfo.fsPath.data(), fs))
		{
			const auto& resourcePath = m_pathWidget->getSelectedPathItem()->getResourceDirectory() + "/" + m_buffer.data();
			V_LOG_INFO(Editor, "add material: {}", resourcePath);
			V_LOG_INFO(Editor, " vertex shader {}", vs);
			V_LOG_INFO(Editor, " fragment shader: {}", fs);
			auto material = std::make_shared<Material>();
			material->setShaderPath(vs, fs);
			material->setResourcePath(resourcePath);
			ResourceRegistry::Instance().save(material);
			return true;
		}
		return false;
	}

	void AddResourceDialog::buildMaterialInput()
	{
		ImGui::Text("vertex shader  :");
		ImGui::SameLine();
		ImGui::InputTextWithHint("##vertex_shader", "path", m_materialInfo.vsPath.data(), m_materialInfo.vsPath.size());

		ImGui::Text("fragment shader:");
		ImGui::SameLine();
		ImGui::InputTextWithHint("##fragment_shader", "path", m_materialInfo.fsPath.data(), m_materialInfo.fsPath.size());
	}
}
