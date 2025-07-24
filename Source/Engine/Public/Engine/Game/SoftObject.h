#ifndef __volucris_soft_object__
#define __volucris_soft_object__

#include <Engine/Asset/AssetManager.h>

namespace volucris
{
	template <typename T>
	class SoftObject
	{
	public:
		SoftObject(std::string path = "")
			: m_path(std::move(path))
			, m_object(nullptr)
		{

		}

		std::shared_ptr<T> tryLoad()
		{
			m_object = AssetManager::getInstance().loadAsset<T>(m_path);
			return m_object;
		}

		std::shared_ptr<T> object()
		{
			return m_object;
		}

		T* operator->() const
		{
			return m_object.get();
		}

		bool operator==(T* obj) const
		{
			return m_object.get() == obj;
		}

		explicit operator bool() const
		{
			return m_object != nullptr;
		}

		template <class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& m_path;
		}

		bool isValid() const { return !m_path.empty(); }

	private:
		std::string m_path;
		std::shared_ptr<T> m_object;
	};
}

#endif // !__volucris_soft_object_path__
