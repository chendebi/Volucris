#include <Game/Material.h>

namespace volucris
{
	Material::Material()
		: GameObject()
		, m_vss()
		, m_fss()
	{
	}

	Material::Material(const std::string& vss, const std::string fss)
		: GameObject()
		, m_vss(vss)
		, m_fss(fss)
	{
	}

	MaterialFloatParameter& Material::addParameter(const std::string& name, float value)
	{
		m_floatParameters.push_back({ name, value });
		return *m_floatParameters.rbegin();
	}

	MaterialVector4Parameter& Material::addParameter(const std::string& name, glm::vec4 value)
	{
		m_vec4Parameters.push_back({ name, value });
		return *m_vec4Parameters.rbegin();
	}

	bool Material::setFloatParameter(const std::string& name, float value)
	{
		for (auto& param : m_floatParameters)
		{
			if (param.getName() == name)
			{
				param.setValue(value);
				return true;
			}
		}
		return false;
	}

	bool Material::setVector4Parameter(const std::string& name, const glm::vec4& value)
	{
		for (auto& param : m_vec4Parameters)
		{
			if (param.getName() == name)
			{
				param.setValue(value);
				return true;
			}
		}
		return false;
	}


}

BOOST_CLASS_EXPORT_IMPLEMENT(volucris::Material)