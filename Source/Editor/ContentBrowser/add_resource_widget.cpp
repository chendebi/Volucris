#include "add_resource_widget.h"
#include <imgui.h>
#include <Engine/Resource/resource_registry.h>
#include <Engine/Resource/material.h>
#include <Engine/Resource/material_resource.h>
#include <Engine/Resource/material.h>
#include "path_tree_widget.h"
#include "EditorEntry/editor_core.h"
#include <iostream>
#include "material_loader.h"
#include <Engine/Application/file_dialog.h>

namespace volucris
{
	static const char* ResourceTypes[] = { "Material", "StaticMesh", "Texture(Unsupport)" };

	AddResourceDialog::AddResourceDialog(PathTreeWidget* pathWidget)
		: Widget()
		, m_opened(false)
		, m_showHint(false)
		, m_currentIdx(0)
		, m_buffer()
		, m_filePath0()
		, m_filePath1()
		, m_pathWidget(pathWidget)
	{
		m_buffer.resize(64);
		m_filePath0.resize(_MAX_PATH);
		m_filePath1.resize(_MAX_PATH);
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
			else if (m_currentIdx == 1)
			{
				buildMeshInput();
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
		if (ResourcePath::SystemPathToResourcePath(m_filePath0.data(), vs)
			&& ResourcePath::SystemPathToResourcePath(m_filePath1.data(), fs))
		{
			MaterialLoader loader;
			auto resource = loader.load(vs, fs);
			if (!resource)
			{
				V_LOG_WARN(Editor, "add material failed. shader file not valid.");
				return false;
			}

			const auto& resourcePath = m_pathWidget->getSelectedPathItem()->getResourceDirectory() + "/" + m_buffer.data();
			V_LOG_INFO(Editor, "add material: {}", resourcePath);
			V_LOG_INFO(Editor, " vertex shader {}", vs);
			V_LOG_INFO(Editor, " fragment shader: {}", fs);
			auto material = std::make_shared<Material>();
			material->setShaderPath(vs, fs);
			material->setResourcePath(resourcePath);
			material->setMaterialResource(resource);
			ResourceRegistry::Instance().registry(material, resourcePath);
			ResourceRegistry::Instance().save(material);
			return true;
		}
		return false;
	}

	void AddResourceDialog::buildMaterialInput()
	{
		ImGui::Text("vertex shader  :");
		ImGui::SameLine();
		ImGui::InputTextWithHint("##vertex_shader", "path", m_filePath0.data(), m_filePath0.size());
		ImGui::SameLine();
		if (ImGui::Button("open##vbtn"))
		{
			std::string vsPath;
			ResourceRegistry::Instance().getSystemPathByResourcePath("/Shader", vsPath);
			const char* filter =
				"Vertex File (*.vert)\0*.vert\0";
			const auto filepath = FileDialog::GetOpenFilePath(filter, "select vertex shader file", vsPath.c_str());
			memcpy(m_filePath0.data(), filepath.c_str(), filepath.size() + 1);
		}

		ImGui::Text("fragment shader:");
		ImGui::SameLine();
		ImGui::InputTextWithHint("##fragment_shader", "path", m_filePath1.data(), m_filePath1.size());
		ImGui::SameLine();
		if (ImGui::Button("open##fbtn"))
		{
			std::string fsPath;
			ResourceRegistry::Instance().getSystemPathByResourcePath("/Shader", fsPath);
			const char* filter =
				"Fragment File (*.frag)\0*.frag\0";
			const auto filepath = FileDialog::GetOpenFilePath(filter, "select fragment shader file", fsPath.c_str());
			memcpy(m_filePath1.data(), filepath.c_str(), filepath.size() + 1);
		}
	}

	void AddResourceDialog::buildMeshInput()
	{
		ImGui::Text("filepath       :");
		ImGui::SameLine();
		ImGui::InputTextWithHint("##file_path", "path", m_filePath0.data(), m_filePath0.size());

		ImGui::SameLine();
		if (ImGui::Button("open"))
		{
			const char* filter =
				"3D Model Files (*.obj, *.fbx)\0*.obj;*.fbx\0"
				"OBJ Files (*.obj)\0*.obj\0"
				"FBX Files (*.fbx)\0*.fbx\0"
				"All Files (*.*)\0*.*\0";
			const auto filepath = FileDialog::GetOpenFilePath(filter, "select mesh file", m_filePath0.data());
			memcpy(m_filePath0.data(), filepath.c_str(), filepath.size() + 1);
		}
	}

	void AddResourceDialog::buildPathSelector(const char* filter, const char* title, std::vector<char>& path)
	{
		if (ImGui::Button("open"))
		{
			const auto filepath = FileDialog::GetOpenFilePath(filter, title, "");
			memcpy(path.data(), filepath.c_str(), filepath.size() + 1);
		}
	}
}
