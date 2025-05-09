#ifndef __volucris_string_help_h__
#define __volucris_string_help_h__

#include <string>
#include <fmt/format.h>

namespace volucris
{
	namespace StringHelp
	{
		enum Encoding
		{
			ANSI,
			UTF_8 = 65001
		};

		std::wstring strToWideStr(const std::string& str, Encoding encode=ANSI);
	}

	inline std::string operator/(const std::string& lhs, const std::string& rhs)
	{
		return fmt::format("{}/{}", lhs, rhs);
	}
}

#endif // !__volucris_string_help_h__
