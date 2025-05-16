#include "Resource/resource_manager.h"
#include "Core/volucris.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <regex>
#include <Resource/material.h>

namespace volucris
{
	MaterialParameterDesc::Type getTypeByString(const std::string& name)
	{
		MaterialParameterDesc::Type type = MaterialParameterDesc::UNKNOWN;
		if (name == "float") { type = MaterialParameterDesc::FLOAT; }
		else if (name == "vec3") { type = MaterialParameterDesc::VEC3; }
		else if (name == "mat4") { type = MaterialParameterDesc::MAT4; }

		if (type == MaterialParameterDesc::UNKNOWN)
		{
			V_LOG_WARN(Engine, "load material with unsupported uniform type: {}", name);
		}
		return type;
	}

	using MaterialParameterMap = std::unordered_map<MaterialParameterDesc::Type, std::vector<std::string>>;

	static bool getShaderSource(const std::string& filePath, std::string& source, MaterialParameterMap& map)
	{
		V_LOG_DEBUG(Engine, "load shader source: {}", filePath)
		std::ifstream file(filePath);
		if (!file.is_open())
		{
			V_LOG_WARN(Engine, "open file failed.");
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
				MaterialParameterDesc::Type type = MaterialParameterDesc::UNKNOWN;    // "mat4"
				std::string name = matches[2];    // "v_modelMat"

				if (name == MODEL_MATRIX_UNIFORM_NAME)
				{
					type = MaterialParameterDesc::MODEL_INFO;
				}
				else
				{
					type = getTypeByString(matches[1]);
				}

				if (type != MaterialParameterDesc::UNKNOWN)
				{
					V_LOG_DEBUG(Engine, "find material parameter: {} - {}", matches[1].str(), name);
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
				else
				{
					type = MaterialParameterDesc::UNKNOWN;
				}

				if (type != MaterialParameterDesc::UNKNOWN)
				{
					V_LOG_DEBUG(Engine, "find material uniform block: {}", name);
					map[type].push_back(name);
				}
				else
				{
					V_LOG_DEBUG(Engine, "find unsupported uniform block: {}", name);
				}
			}

			++lineCount;
			res += line + "\n";
		}
		source = std::move(res);
		return true;
	}

	ResourceManager::~ResourceManager()
	{

	}

	std::shared_ptr<Material> ResourceManager::getMaterialFromPath(const ResourcePath& path)
	{
		if (path.type != "mat")
		{
			return nullptr;
		}

		std::shared_ptr<MaterialResource> resource = nullptr;
		{
			auto it = m_resources.find(path.fullpath);
			if (it != m_resources.end())
			{
				resource = std::dynamic_pointer_cast<MaterialResource>(it->second);
			}
		}

		if (!resource)
		{
			auto vsf = path.getSystemPath("vert");
			auto fsf = path.getSystemPath("frag");

			V_LOG_DEBUG(Engine, "load material")
			V_LOG_DEBUG(Engine, " vertex shader:   {}", vsf)
			V_LOG_DEBUG(Engine, " fragment shader: {}", fsf)

			MaterialParameterMap map;
			std::string vss, fss;
			if (!getShaderSource(vsf, vss, map) || !getShaderSource(fsf, fss, map))
			{
				V_LOG_WARN(Engine, "load material failed.");
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

			resource = std::make_shared<MaterialResource>(vss, fss);
			resource->setParameters(descs);
			resource->setResourcePath(path);

			m_resources[path.fullpath] = resource;
			V_LOG_DEBUG(Engine, "load material success.")
		}

		auto mat = std::make_shared<Material>(resource);
		return mat;
	}
}
