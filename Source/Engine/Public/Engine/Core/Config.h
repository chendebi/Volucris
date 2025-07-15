#ifndef __volucris_config_h__
#define __volucris_config_h__

#include <string>
#include <memory>

namespace volucris
{
	class Config
	{
	public:
		Config();

		Config(const std::string& filepath);

		~Config();

		void setValue(const std::string& name, int value);

		void setValue(const std::string& name, float value);

		void setValue(const std::string& name, const std::string& value);

		int getValue(const std::string& name, int defaultValue = 0) const;

		float getValue(const std::string& name, float defaultValue = 0.0f) const;

		std::string getValue(const std::string& name, const std::string& defaultValue = "") const;

		bool save() const;

		bool save(const std::string& filepath) const;

	private:
		struct Impl;
		Impl* m_impl;
	};
}

#endif // !__volucris_config_h__
