#include <Core/Config.h>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <FileSystem/FileSystem.h>

namespace boostpt = boost::property_tree;

namespace volucris
{
	struct Config::Impl
	{
		boostpt::ptree pt;
		std::string filepath;
	};

	Config::Config()
		: m_impl(new Impl)
	{
	}

	Config::Config(const std::string& filepath)
		: Config()
	{
		m_impl->filepath = gFileSystem.virtualToPhysical(filepath);
		try {
			boostpt::read_ini(m_impl->filepath, m_impl->pt);
		}
		catch(boostpt::ini_parser_error& e)
		{

		}
	}

	Config::~Config()
	{
		delete m_impl;
	}

	void Config::setValue(const std::string& name, int value)
	{
		m_impl->pt.put(name, value);
	}

	void Config::setValue(const std::string& name, float value)
	{
		m_impl->pt.put(name, value);
	}

	void Config::setValue(const std::string& name, const std::string& value)
	{
		m_impl->pt.put(name, value);
	}

	int Config::getValue(const std::string& name, int defaultValue) const
	{
		return m_impl->pt.get<int>(name, defaultValue);
	}

	float Config::getValue(const std::string& name, float defaultValue) const
	{
		return m_impl->pt.get<float>(name, defaultValue);
	}

	std::string Config::getValue(const std::string& name, const std::string& defaultValue) const
	{
		return m_impl->pt.get<std::string>(name, defaultValue);
	}

	bool Config::save() const
	{
		if (m_impl->filepath.empty())
		{
			return false;
		}
		boostpt::write_ini(m_impl->filepath, m_impl->pt);
		return true;
	}

	bool Config::save(const std::string& filepath) const
	{
		m_impl->filepath = gFileSystem.virtualToPhysical(filepath);
		return save();
	}
}