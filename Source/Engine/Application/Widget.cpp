#include "Application/Widget.h"
#include <Core/VectorHelp.h>
#include <Core/Volucris.h>
#include <imgui/imgui.h>

namespace volucris
{
	Widget::Widget()
		: Object()
		, m_parent(nullptr)
		, m_children()
	{

	}

	Widget::~Widget()
	{
		if (m_parent)
		{
			setParent(nullptr);
		}

		for (auto& child : m_children)
		{
			child->m_parent = nullptr;
			child->parentChanged(this, nullptr);
		}

		m_children.clear();
	}

	void Widget::addChild(std::shared_ptr<Widget> child)
	{
		if (child)
		{
			child->setParent(this);
		}
	}

	void Widget::setParent(Widget* parent)
	{
		if (m_parent != parent)
		{
			auto oldTopWidget = getTopWidget();
			if (m_parent)
			{
				VectorHelp::quickRemove<Widget>(m_parent->m_children, this);
			}
			auto oldParent = m_parent;
			m_parent = parent;
			if (m_parent)
			{
				m_parent->m_children.push_back(getShared<Widget>());
			}
			parentChanged(oldParent, m_parent);
			topWidgetChanged(oldTopWidget, getTopWidget());
		}
	}

	void Widget::build()
	{
		onBuild();

		for (const auto& child : m_children)
		{
			child->build();
		}
	}


	void Widget::topWidgetChanged(Widget* old, Widget* current)
	{
		onTopWidgetChanged(old, current);
		for (const auto& child : m_children)
		{
			child->topWidgetChanged(old, current);
		}
	}
}
