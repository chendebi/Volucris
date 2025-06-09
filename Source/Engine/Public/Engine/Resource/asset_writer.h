#ifndef __volucris_asset_writer_h__
#define __volucris_asset_writer_h__

#include <memory>

namespace volucris
{
	class ResourceObject;

	class AssetWriter
	{
	public:
		bool save(const std::shared_ptr<ResourceObject>& object);

	private:

	};
}

#endif // !__volucris_asset_writer_h__
