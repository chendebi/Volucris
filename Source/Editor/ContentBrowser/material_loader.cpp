#include "material_loader.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <regex>
#include "EditorEntry/editor_core.h"
#include <Engine/Resource/material.h>
#include <Engine/Core/material_global.h>

namespace volucris
{
	using MaterialParameterMap = std::unordered_map<MaterialParameterType, std::vector<std::string>>;

	static MaterialParameterType getTypeByString(const std::string& name)
	{
		MaterialParameterType type = MaterialParameterType::NONE;
		if (name == "float") { type = MaterialParameterType::FLOAT; }
		else if (name == "vec3") { type = MaterialParameterType::VEC3; }
		else if (name == "sampler2D") { type = MaterialParameterType::TEXTURE2D; }

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

				type = getTypeByString(matches[1]);

				if (type != MaterialParameterType::NONE)
				{
					V_LOG_DEBUG(Editor, "find material parameter: {} - {}", matches[1].str(), name);
					map[type].push_back(name);
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

		auto resource = std::make_shared<MaterialResource>(vss, fss);
		for (const auto& [type, names] : map)
		{
			if (type == MaterialParameterType::NONE)
			{
				continue;
			}

			for (const auto& name : names)
			{
				resource->addParameter(name, type);
			}
		}

		return resource;
	}

	bool MaterialLoader::reload(Material* material)
	{
		/*const auto& vsf = material->getVertexShaderFilePath();
		const auto& fsf = material->getFragmentShaderFilePath();

		std::string vsp, fsp;
		ResourcePath::ResourcePathToSystemPath(vsf, vsp);
		ResourcePath::ResourcePathToSystemPath(fsf, fsp);

		MaterialLoader loader;
		if (auto resource = loader.load(vsp, fsp))
		{
			material->setMaterialResource(resource);
			return true;
		}*/
		return false;
	}
}
