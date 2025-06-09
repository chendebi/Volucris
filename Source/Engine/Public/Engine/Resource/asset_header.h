#ifndef __volucris_asset_header_h__
#define __volucris_asset_header_h__

#include <string>

namespace volucris
{
	struct AssetHeader
	{
		char magic[8];
		int version;

		bool operator==(const AssetHeader& header) const
		{
			return (strncmp(magic, header.magic, 8) == 0) && (version == header.version);
		}

		bool operator!=(const AssetHeader& other) const
		{
			return !(operator==(other));
		}

		static AssetHeader VerifyHeader;
	};

	//extern AssetHeader VerifyHeader;
}

#endif // !__volucris_asset_header_h__
