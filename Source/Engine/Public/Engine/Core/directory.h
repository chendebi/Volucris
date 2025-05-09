#ifndef __volucris_directory_h__
#define __volucris_directory_h__

#include "Engine/Core/string_help.h"

namespace volucris
{
	class Directory
	{
	public:
		Directory() = default;

		Directory(const std::string& dirpath);

		static Directory ApplicationDirectory();

		static Directory EngineDirectory();

		Directory absolute() const;

		std::string toString() const { return m_path; }

		inline Directory operator/(const std::string& subname)
		{
			return m_path / subname;
		}

	private:
		std::string m_path;
	};
}

#endif // !__volucris_directory_h__
