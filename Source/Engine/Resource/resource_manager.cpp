#include "Resource/resource_manager.h"
#include "Core/volucris.h"
#include <fstream>
#include <sstream>
#include <vector>

namespace volucris
{
	static std::string getShaderSource(const std::string& filePath, MaterialParameterMap& map)
	{
		std::ifstream file(filePath);
		if (!file.is_open())
		{
			V_LOG_WARN(Engine, "open file {} failed.", filePath);
			return std::string();
		}

		std::string res;
		std::string line;
		while (std::getline(file, line))
		{
			std::istringstream iss(line);
			std::vector<std::string> tokens;
			std::string token;

			while (iss >> token)
			{
				tokens.push_back(token);
			}

			if (tokens.size() >= 3 && tokens[0] == "uniform")
			{
				auto type = tokens[1];
				auto name = tokens[2];
				auto parameter = createMaterialParameter(type, name);
				if (parameter)
				{
					map[name] = std::move(parameter);
				}
			}

			res += line + "\n";
		}
		return res;
	}

	ResourceManager::~ResourceManager()
	{

	}

	std::shared_ptr<ResourceObject> ResourceManager::getResourceObject(const std::string& assetPath)
	{
		auto it = m_resources.find(assetPath);
		if (it != m_resources.end())
		{
			return it->second;
		}

		auto path = ResourcePath(assetPath);
		if (path.type == "mat")
		{

		}
	}

	std::shared_ptr<Material> ResourceManager::loadMaterial(const ResourcePath& path)
	{
		auto vss
	}
}
