#ifndef __volucris_scene_object_h__
#define __volucris_scene_object_h__

#include <string>
#include <rttr/registration.h>
#include <rttr/rttr_enable.h>

namespace volucris
{
	class Scene;
	class SceneObject
	{
		RTTR_ENABLE()
	public:
		SceneObject() : m_scene(nullptr) {}

		virtual ~SceneObject();

		Scene* getScene() const { return m_scene; }

		bool isAttached() const { return m_scene; }

		void attach(Scene* scene);

		void disattach();

		void setDisplayName(const std::string& name) { m_displayName = name; }

		std::string getDisplayName() const { return m_displayName; }
	protected:
		virtual void attached() {}

		virtual void disattached(Scene* scene) {}

	private:
		Scene* m_scene;
		std::string m_displayName;
	};
}

#endif // !__volucris_scene_object_h__
