#ifndef __volucris_component_h__
#define __volucris_component_h__

#include <memory>
#include <vector>
#include "Engine/Scene/scene_object.h"
#include "Engine/Core/types_help.h"

namespace volucris
{
	class Component : public SceneObject
	{
		RTTR_ENABLE(SceneObject)
	public:
		Component();

		virtual ~Component() = default;

		void update();

		void addChild(const std::shared_ptr<Component>& child);

		Component* getParentComponent() const { return m_parent; }

		void markRenderStateDirty() { m_renderStateDirty = true; }

		void markRenderTransformDirty() { m_renderTransformDirty = true; }

		void markRenderDynamicDataDirty() { m_renderDynamicDataDirty = true; }

		bool isRenderTransformDirty() const { return m_renderTransformDirty; }

	protected:
		virtual void updateDynamicData() {}

		virtual void updateTransform() {}

		virtual void updateRenderState() {}

		virtual void releaseRenderData() {}

		void attached() override;

		void disattached(Scene* scene) override;

	private:
		uint8 m_renderStateDirty;
		uint8 m_renderTransformDirty;
		uint8 m_renderDynamicDataDirty;
		Component* m_parent;
		std::vector<std::shared_ptr<Component>> m_children;
	};
}

#endif // !__volucris_component_h__
