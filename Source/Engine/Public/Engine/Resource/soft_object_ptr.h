#ifndef __volucris_soft_object_ptr_h__
#define __volucris_soft_object_ptr_h__

#include <memory>
#include <string>
#include <Engine/Resource/asset_path.h>
#include <Engine/Core/serializer.h>

namespace volucris
{
	template<typename T>
	class TSoftObjectPtr
	{
	public:
		TSoftObjectPtr()
			: m_path()
			, m_object()
		{ }

		TSoftObjectPtr(std::string path)
			: m_path(std::move(path))
			, m_object()
		{ }

		TSoftObjectPtr(const std::shared_ptr<T>& object)
			: m_path(object->getAsset().assetPath)
			, m_object(object)
		{

		}

		std::shared_ptr<T> tryLoad()
		{
			auto object = m_object.lock();
			if (!object)
			{
				Asset asset(m_path);
				object = std::dynamic_pointer_cast<T>(asset.load());
				m_object = object;
			}
			return object;
		}

		void serialize(Serializer& serializer)
		{
			serializer.serialize(m_path);
		}

		void deserialize(Serializer& serializer)
		{
			std::string path;
			if (serializer.deserialize(path))
			{
				m_path = std::move(path);
				m_object.reset(); // Reset the weak pointer to force reloading
			}
		}

	private:
		std::string m_path;
		std::weak_ptr<T> m_object;
	};
}

#endif // !__volucris_soft_object_ptr_h__
