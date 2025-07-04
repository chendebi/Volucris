#ifndef __volucris_asset_writer_h__
#define __volucris_asset_writer_h__

#include <memory>

namespace volucris
{
	class Package;

	class AssetWriter
	{
	public:
		AssetWriter(std::shared_ptr<Package> packgae);
		
		~AssetWriter();
		
		bool write();

	private:
		std::shared_ptr<Package> m_package;
	};
}

#endif // !__volucris_asset_writer_h__
