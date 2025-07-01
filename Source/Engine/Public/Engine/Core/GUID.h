#ifndef __volucris_guid_h__
#define __volucris_guid_h__

#include <Engine/Core/TypesHelp.h>
#include <string>
#include <fmt/format.h>

namespace volucris
{
	struct GUID
	{
		unsigned long  Data1;
		unsigned short Data2;
		unsigned short Data3;
		unsigned char  Data4[8];

		std::string toString() const {
			char buffer[64];
			snprintf(buffer, sizeof(buffer),
				"%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X",
				Data1, Data2, Data3,
				Data4[0], Data4[1],
				Data4[2], Data4[3],
				Data4[4], Data4[5],
				Data4[6], Data4[7]);
			return std::string(buffer);
		}

		static GUID generate();
	};
}

// 格式化vector
template <>
struct fmt::formatter<volucris::GUID> {
	constexpr auto parse(format_parse_context& ctx) {
		return ctx.begin();
	}

	auto format(const volucris::GUID& guid, format_context& ctx) const {
		auto out = ctx.out();
		return fmt::format_to(out, "{{{}}}", guid.toString());
	}
};

#endif // !__volucris_guid_h__
