#include "Scene/component.h"
#include "Core/vector_help.h"
#include "Core/volucris.h"

namespace volucris
{
	Component::Component()
		: m_renderStateDirty(0)
		, m_renderTransformDirty(0)
		, m_renderDynamicDataDirty(0)
		, m_parent(nullptr)
		, m_children()
	{

	}

	void Component::update()
	{
		if (m_renderStateDirty)
		{
			updateRenderState();
			m_renderStateDirty = false;
			m_renderDynamicDataDirty = false;
			m_renderTransformDirty = false;
		}
		else
		{
			if (m_renderDynamicDataDirty)
			{
				updateDynamicData();
				m_renderDynamicDataDirty = false;
			}

			if (m_renderTransformDirty)
			{
				updateTransform();
				m_renderTransformDirty = false;
			}
		}
		
		for (const auto& child : m_children)
		{
			child->update();
		}
	}

	void Component::addChild(const std::shared_ptr<Component>& child)
	{
		if (!child || child.get() == this)
		{
			return;
		}

		if (child->m_parent)
		{
			V_LOG_WARN(Engine, "todo: support dynamic change component parent.");
			return;
		}

		if (child->m_parent == this)
		{
			return;
		}

		if (child->m_parent)
		{
			VectorHelp::quickRemove<std::shared_ptr<Component>>(child->m_parent->m_children, child);
		}
		child->m_parent = this;
		m_children.push_back(child);

		if (auto scene = getScene())
		{
			child->attach(scene);
			child->markRenderStateDirty();
		}
	}

	void Component::disattached(Scene* scene)
	{
		markRenderStateDirty();
	}
}
