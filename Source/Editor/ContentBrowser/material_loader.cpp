#include "material_loader.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <regex>
#include "EditorEntry/editor_core.h"
#include <Engine/Resource/material.h>

namespace volucris
{
	using MaterialParameterMap = std::unordered_map<MaterialParameterType, std::vector<std::string>>;

	constexpr std::string_view MODEL_MATRIX_UNIFORM_NAME = "v_modelMat";
	constexpr std::string_view CAMERA_INFO_BLOCK_NAME = "v_cameraInfo";
	constexpr std::string_view DIRECTION_LIGHT_BLOCK_NAME = "v_directionLight";

	static MaterialParameterType getTypeByString(const std::string& name)
	{
		MaterialParameterType type = MaterialParameterType::NONE;
		if (name == "float") { type = MaterialParameterType::FLOAT; }
		else if (name == "vec3") { type = MaterialParameterType::VEC3; }

		if (type == MaterialParameterType::NONE)
		{
			V_LOG_WARN(Editor, "load material with unsupported uniform type: {}", name);
		}
		return type;
	}

	static bool getShaderSource(const std::string& filePath, std::string& source, MaterialParameterMap& map)
	{
		V_LOG_DEBUG(Editor, "load shader source: {}", filePath)
			std::ifstream file(filePath);
		if (!file.is_open())
		{
			V_LOG_WARN(Editor, "open file failed.");
			return false;
		}

		std::string res;
		std::string line;
		size_t lineCount = 1;
		while (std::getline(file, line))
		{
			std::regex uboPattern(R"(uniform\s+(\w+)(?:\s*\{|\s*;)?)");
			std::regex uniformattern(R"(uniform\s+(\w+)\s+(\w+)\s*;)");

			std::smatch matches;

			if (std::regex_search(line, matches, uniformattern))
			{
				MaterialParameterType type = MaterialParameterType::NONE;    // "mat4"
				std::string name = matches[2];    // "v_modelMat"

				if (name == MODEL_MATRIX_UNIFORM_NAME)
				{
					type = MaterialParameterDesc::MODEL_INFO;
				}
				else
				{
					type = getTypeByString(matches[1]);
				}

				if (type != MaterialParameterType::NONE)
				{
					V_LOG_DEBUG(Editor, "find material parameter: {} - {}", matches[1].str(), name);
					map[type].push_back(name);
				}
			}
			else if (std::regex_search(line, matches, uboPattern))
			{
				MaterialParameterDesc::Type type = MaterialParameterDesc::UNKNOWN;    // "mat4"
				std::string name = matches[1];    // "v_modelMat"

				if (name == CAMERA_INFO_BLOCK_NAME)
				{
					type = MaterialParameterDesc::CAMERA_INFO;
				}
				else if (name == DIRECTION_LIGHT_BLOCK_NAME)
				{
					type = MaterialParameterDesc::DIRECTION_LIGHT;
				}
				else
				{
					type = MaterialParameterDesc::UNKNOWN;
				}

				if (type != MaterialParameterDesc::UNKNOWN)
				{
					V_LOG_DEBUG(Editor, "find material uniform block: {}", name);
					map[type].push_back(name);
				}
				else
				{
					V_LOG_DEBUG(Editor, "find unsupported uniform block: {}", name);
				}
			}

			++lineCount;
			res += line + "\n";
		}
		source = std::move(res);
		return true;
	}

	std::shared_ptr<MaterialResource> MaterialLoader::load(const std::string& vsf, const std::string& fsf)
	{
		MaterialParameterMap map;
		std::string vss, fss;
		if (!getShaderSource(vsf, vss, map) || !getShaderSource(fsf, fss, map))
		{
			V_LOG_WARN(Editor, "load material failed.");
			return nullptr;
		}

		std::vector<MaterialParameterDesc> descs;
		size_t offset = 0;
		for (const auto& [type, names] : map)
		{
			if (type == MaterialParameterDesc::UNKNOWN)
			{
				continue;
			}

			auto typeSize = MaterialParameterDesc::sizeOfType(type);
			for (const auto& name : names)
			{
				MaterialParameterDesc desc;
				desc.name = name;
				desc.type = type;
				desc.offset = offset;
				descs.push_back(desc);
				offset += typeSize;
			}
		}

		auto resource = std::make_shared<MaterialResource>(vss, fss);
		resource->setParameters(descs);
		return resource;
	}

	bool MaterialLoader::reload(Material* material)
	{
		const auto& vsf = material->getVertexShaderFilePath();
		const auto& fsf = material->getFragmentShaderFilePath();

		std::string vsp, fsp;
		ResourcePath::ResourcePathToSystemPath(vsf, vsp);
		ResourcePath::ResourcePathToSystemPath(fsf, fsp);

		MaterialLoader loader;
		if (auto resource = loader.load(vsp, fsp))
		{
			material->setMaterialResource(resource);
			return true;
		}
		return false;
	}
}
