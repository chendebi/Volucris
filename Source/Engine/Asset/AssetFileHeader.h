#ifndef __volucris_asset_file_header_h__
#define __volucris_asset_file_header_h__

#include <boost/serialization/array.hpp>

namespace volucris
{
	// 资源文件头部信息
	struct AssetFileHeader
	{
		static constexpr const char* MAGIC_KEY = "VOLUCRIS_ASSET";
		static constexpr int VERSION = 1;
		std::array<char, 16> magic; // 魔法字节，用于标识文件类型
		int version;    // 文件版本号

		AssetFileHeader(bool inited = false)
		{
			if (inited)
			{
				std::copy(MAGIC_KEY, MAGIC_KEY + 16, magic.begin());
				version = VERSION;
			}
			else
			{
				std::fill(magic.begin(), magic.end(), 0);
				version = 0;
			}
		}

		template <class Archive>
		void serialize(Archive& ar, const unsigned int ver)
		{
			ar& magic;
			ar& version;
		}
	};
} // namespace volucris

#endif // !__volucris_asset_file_header_h__
