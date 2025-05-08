#ifndef __volucris_actor_h__
#define __volucris_actor_h__

#include <vector>
#include "component.h"
#include "Engine/Scene/scene_object.h"

namespace volucris
{
	class Actor : public SceneObject
	{
	public:
		Actor();

		virtual ~Actor() = default;
		
		void setRootComponent(const std::shared_ptr<Component>& component)
		{
			m_rootComponent = component;
		}

		std::shared_ptr<Component> getRootComponent() const
		{
			return m_rootComponent;
		}

		bool addComponent(const std::shared_ptr<Component>& component);

		virtual void tick(double delta) {}

		void update();

	protected:
		virtual void onUpdate() {}

		void attached() override;

		void disattached(Scene* scene) override;

	private:
		std::shared_ptr<Component> m_rootComponent;
	};
}

#endif // !__volucris_actor_h__
