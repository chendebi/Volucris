#ifndef __volucris_game_object_h__
#define __volucris_game_object_h__

#include <Engine/Core/Object.h>
#include <vector>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/export.hpp>
#include <rttr/registration.h>
#include <rttr/rttr_enable.h>

namespace volucris
{
	class Package;

	class GameObject : public Object
	{
		RTTR_ENABLE()
	public:
		GameObject();

		virtual ~GameObject() override;

		void setParent(GameObject* parent);

		GameObject* getParent() const
		{
			return m_parent;
		}

		const std::vector<std::shared_ptr<GameObject>>& getChildren() const
		{
			return m_children;
		}

		GameObject* getTopParent() 
		{
			GameObject* topParent = this;
			while (topParent->m_parent)
			{
				topParent = topParent->m_parent;
			}
			return topParent;
		}

		template <class Archive>
		void serialize(Archive& ar, const unsigned int version) 
		{
		}

		void setDisplayName(const std::string& name)
		{
			m_displayName = name;
		}

		const std::string& getDisplayName() const
		{
			return m_displayName;
		}

	protected:
		void addDependence(const std::string& path);

		void removeDependence(const std::string& path);

	private:
		GameObject* m_parent;
		std::vector<std::shared_ptr<GameObject>> m_children;
		std::vector<std::string> m_dependences;
		std::string m_displayName;
	};
}

#endif // !__volucris_game_object_h__
