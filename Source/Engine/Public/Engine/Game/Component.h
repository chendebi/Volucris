#ifndef __volucris_component_h__
#define __volucris_component_h__

#include <Engine/Game/GameObject.h>
#include <Engine/Core/TypesHelp.h>

namespace volucris
{
	class Entity;

	class Component : public GameObject
	{
		RTTR_ENABLE(GameObject)

	public:
		Component();

		void attachTo(Entity* entity);

		void markRenderStateDirty()
		{
			m_dirtyFlags |= DirtyFlag::RenderState;
		}

		void markTransformDirty()
		{
			m_dirtyFlags |= DirtyFlag::TransformState;
		}

		virtual void update();

	protected:
		virtual void onRenderStateChanged() {}

		virtual void onTransformStateChanged() {}

	private:
		friend class Entity;
		enum DirtyFlag
		{
			RenderState = 0x01,
			TransformState = 0x02,
		};
		uint8 m_dirtyFlags;

		Entity* m_entity;
	};
}

#endif // !__volucris_icomponent_h__
