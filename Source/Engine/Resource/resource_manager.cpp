#include "Resource/resource_manager.h"
#include "Core/volucris.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <Resource/material.h>

namespace volucris
{
	MaterialParameter::Type getTypeByString(const std::string& name)
	{
		MaterialParameter::Type type = MaterialParameter::UNKNOWN;
		if (name == "float") { type = MaterialParameter::FLOAT; }
		else if (name == "vec3") { type = MaterialParameter::VEC3; }

		if (type == MaterialParameter::UNKNOWN)
		{
			V_LOG_WARN(Engine, "load material with unsupported uniform type: {}", name);
		}
		return type;
	}

	static bool getShaderSource(const std::string& filePath, std::string& source, MaterialResource::MaterialParameterMap& map)
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
			std::istringstream iss(line);
			std::vector<std::string> tokens;
			std::string token;

			while (iss >> token)
			{
				tokens.push_back(token);
			}

			if (!tokens.empty() && tokens[0] == "uniform")
			{
				if (tokens.size() < 3)
				{
					V_LOG_WARN(Engine, "shader source parase failed. line: {}", lineCount);
					return false;
				}

				auto type = getTypeByString(tokens[1]);
				auto name = tokens[2];

				
				if (!name.empty() && name[name.length() - 1] == ';')
				{
					name = name.substr(0, name.length() - 1);
				}

				if (name.empty())
				{
					V_LOG_WARN(Engine, "shader source parase failed. line: {}", lineCount);
					return false;
				}

				if (type != MaterialParameter::UNKNOWN)
				{
					V_LOG_DEBUG(Engine, "find material parameter: {} - {}", tokens[1], name)
					map[type].push_back(name);
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

			MaterialResource::MaterialParameterMap map;
			std::string vss, fss;
			if (!getShaderSource(vsf, vss, map) || !getShaderSource(fsf, fss, map))
			{
				V_LOG_WARN(Engine, "load material failed.");
				return nullptr;
			}

			resource = std::make_shared<MaterialResource>(vss, fss);
			resource->setParameters(map);

			m_resources[path.fullpath] = resource;
			V_LOG_DEBUG(Engine, "load material success.")
		}

		auto mat = std::make_shared<Material>(resource);
		return mat;
	}
}
