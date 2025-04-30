#include "Core/string_help.h"

#include <Windows.h>

namespace volucris::StringHelp
{
	std::wstring strToWideStr(const std::string& str, Encoding encoding)
	{
		const char* data = str.c_str();
		int wsize = MultiByteToWideChar(encoding, MB_ERR_INVALID_CHARS, data, str.length(), nullptr, 0);
		wchar_t* wstr = new wchar_t[wsize + 1];
		MultiByteToWideChar(encoding, MB_ERR_INVALID_CHARS, data, str.length(), wstr, wsize);
		wstr[wsize] = '\0';
		std::wstring res = wstr;
		delete[] wstr;
		return res;
	}
}
