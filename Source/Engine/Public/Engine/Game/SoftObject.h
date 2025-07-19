#ifndef __volucris_soft_object__
#define __volucris_soft_object__

#include <Engine/Asset/AssetManager.h>

namespace volucris
{
	template <typename T>
	class SoftObject
	{
	public:
		SoftObject(std::string path)
			: m_path(std::move(path))
			, m_object(nullptr)
		{

		}

		std::shared_ptr<T> tryLoad()
		{
			m_object = AssetManager::getInstance().loadAsset<T>(m_path);
		}

		std::shared_ptr<T> object()
		{
			return m_object;
		}

		T* operator->() const
		{
			return m_object.get();
		}

	private:
		std::string m_path;
		std::shared_ptr<T> m_object;
	};
}

#endif // !__volucris_soft_object_path__
