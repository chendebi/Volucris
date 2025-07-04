#ifndef __volucris_package_h__
#define __volucris_package_h__

#include <Engine/Game/GameObject.h>
#include <string>
#include <Engine/Asset/AssetData.h>
#include <boost/serialization/vector.hpp>

namespace volucris
{
	class Package : public GameObject
	{
	public:
		Package();

		Package(const std::string& path);

		~Package() override;

		const AssetData& getAssetData() const { return m_assetData; }

		template <class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& m_assetData;
			std::vector<std::shared_ptr<GameObject>> objects;
			if (Archive::is_loading::value)
			{
				ar& objects;
				for (auto& object : objects)
				{
					object->setParent(this);
				}
			}
			else
			{
				for (const auto& object : getChildren())
				{
					if (dynamic_cast<Package*>(object.get()))
					{
						continue;
					}
					objects.push_back(object);
				}
				ar& objects;
			}
		}

	private:
		friend class AssetManager;
		AssetData m_assetData; // 包含的资源数据
	};
}

BOOST_CLASS_EXPORT_KEY(volucris::Package)

#endif // !__volucris_package_h__
