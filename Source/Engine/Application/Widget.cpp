#include "Application/Widget.h"
#include <Core/VectorHelp.h>
#include <Core/Volucris.h>

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
			if (m_parent)
			{
				VectorHelp::quickRemove<Widget>(m_parent->m_children, this);
			}
			m_parent = parent;
			if (m_parent)
			{
				m_parent->m_children.push_back(getShared<Widget>());
			}
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
}
