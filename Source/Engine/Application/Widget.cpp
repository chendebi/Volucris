#include "Application/Widget.h"
#include <Core/VectorHelp.h>
#include <Core/Volucris.h>
#include <imgui/imgui.h>
#include <Application/Window.h>
#include <imgui_internal.h>

namespace volucris
{
	bool Widget::hasCustomLayout(const char* id)
	{
		ImGuiContext& g = *GImGui;
		ImGuiWindowSettings* settings = ImGui::FindWindowSettingsByID(ImHashStr(id));
		return (settings != nullptr && settings->Pos.x != FLT_MAX);
	}

	Widget::Widget()
		: Object()
		, m_parent(nullptr)
		, m_children()
		, m_context(nullptr)
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
			auto shared = getShared<Widget>();
			auto oldTopWidget = getTopWidget();
			if (m_parent)
			{
				if (m_context)
				{
					beforeRedererDestroy(m_context);
					m_context = nullptr;
				}
				VectorHelp::quickRemove(m_parent->m_children, shared);
			}

			m_parent = parent;
			if (m_parent)
			{
				m_parent->m_children.push_back(shared);

				if (m_parent->m_context)
				{
					rendererCreated(m_parent->m_context);
				}
			}
		}
	}

	void Widget::build()
	{
		onBuild();
	}

	void Widget::buildChildren()
	{
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

	bool Widget::setDropEvent(DropEvent* event)
	{
		if (onDrop(event))
		{
			return true;
		}

		for (const auto& child : m_children)
		{
			if (child->setDropEvent(event))
			{
				return true;
			}
		}
		return false;
	}

	void Widget::setFocusEvent(FocusEvent* event)
	{
		onWindowFocusChanged(event);
		for (const auto& child : m_children)
		{
			child->setFocusEvent(event);
		}
	}

	void Widget::rendererCreated(RHICommandList* cmdList)
	{
		m_context = cmdList;
		onRendererBuild(cmdList);
		for (const auto& child : m_children)
		{
			child->rendererCreated(cmdList);
		}
	}

	void Widget::beforeRedererDestroy(RHICommandList* cmdList)
	{
		onRendererDestroy(cmdList);
		for (const auto& child : m_children)
		{
			child->beforeRedererDestroy(cmdList);
		}
		m_context = nullptr;
	}
}
