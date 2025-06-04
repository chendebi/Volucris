#ifndef __volucris_asset_reader_h__
#define __volucris_asset_reader_h__

#include <Engine/Resource/asset_path.h>

namespace volucris
{
	class ResourceObject;

	class AssetReader
	{
	public:
		AssetReader()
			: m_path()
		{ }

		void setLoadPath(std::string path)
		{
			m_path = std::move(path);
		}

		std::shared_ptr<ResourceObject> load() const;

		Asset loadAsset() const;

	protected:

	private:
		std::string m_path;
	};
}

#endif // !__volucris_asset_loader_h__
