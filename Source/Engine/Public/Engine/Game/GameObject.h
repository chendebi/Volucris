#ifndef __volucris_game_object_h__
#define __volucris_game_object_h__

#include <Engine/Core/Object.h>
#include <vector>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/export.hpp>

namespace volucris
{
	class Package;

	class GameObject : public Object
	{
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

	protected:
		void addDependence(const std::string& path);

		void removeDependence(const std::string& path);

	private:
		GameObject* m_parent;
		std::vector<std::shared_ptr<GameObject>> m_children;
		std::vector<std::string> m_dependences;
	};
}

#endif // !__volucris_game_object_h__
