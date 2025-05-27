#include <string>
#ifndef __volucris_file_dialog_h__
#define __volucris_file_dialog_h__

namespace volucris
{
	class FileDialog
	{
	public:
		static std::string GetOpenFilePath(const char* filter = nullptr, const char* title = nullptr, const char* initialDirectory = nullptr);
	};
}

#endif // !__volucris_file_dialog_h__
