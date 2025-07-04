#include "Game/Package.h"
#include "Core/VectorHelp.h"
#include "Asset/AssetManager.h"

namespace volucris
{
	Package::Package()
		: GameObject()
		, m_assetData()
	{
		m_assetData.guid = GUID::generate();
	}

	Package::Package(const std::string& path)
		: Package()
	{
		m_assetData.path = path;
	}

	Package::~Package()
	{
	}

} // namespace volucris

BOOST_CLASS_EXPORT_IMPLEMENT(volucris::Package);