#include <Viewport/ViewportWidget.h>
#include <imgui.h>
#include <Engine/Render/View.h>
#include <Engine/Application/Window.h>
#include <Engine/Render/Renderer.h>
#include <Engine/Core/Task.h>

namespace volucris
{
	struct CreateViewTask
	{
		mutable std::unique_ptr<View> view;
		Size size;

		CreateViewTask(std::unique_ptr<View> v, Size s)
			: view(std::move(v)), size(s) {}

		CreateViewTask(const CreateViewTask& task)
		{
			view = std::move(task.view);
			size = task.size;
		}

		CreateViewTask(CreateViewTask&& task) noexcept
			: view(std::move(task.view)), size(task.size) {
		}

		void execute()
		{
			view->resize(size.width, size.height);
			Renderer::getInstance().addView(std::move(view));
		}
	};;

	ViewportWidget::ViewportWidget()
		: Widget()
		, m_view(nullptr)
	{
	}

	void ViewportWidget::onBuild()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::Begin("Preview");
		auto pos = ImGui::GetWindowPos();
		auto winSize = ImGui::GetWindowSize();
		ImVec2 size = ImGui::GetContentRegionAvail();
		Size viewSize = Size((int)size.x, (int)size.y);
		if (viewSize != m_size)
		{
			viewSizeChanged(viewSize);
		}
		ImGui::End();
		ImGui::PopStyleVar();
	}

	void ViewportWidget::onTopWidgetChanged(Widget* widget)
	{
		auto window = dynamic_cast<Window*>(widget);
		if (window && !m_view)
		{
			auto view = std::make_unique<View>();
			m_view = view.get();
			CreateViewTask task = CreateViewTask(std::move(view), m_size);
			Renderer::getInstance().push(createTask(std::move(task)));
		}
		else if (!window && m_view)
		{
			Renderer::getInstance().push([view = m_view]() {
				Renderer::getInstance().removeView(view);
				});
		}
	}

	void ViewportWidget::viewSizeChanged(Size size)
	{
		if (m_view)
		{
			Renderer::getInstance().push([view=m_view, size]() {
				view->resize(size.width, size.height);
				});
		}
	}
}