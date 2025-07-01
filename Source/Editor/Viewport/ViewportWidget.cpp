#include <Viewport/ViewportWidget.h>
#include <imgui.h>
#include <Engine/Render/View.h>
#include <Engine/Application/Window.h>
#include <Engine/Render/Renderer.h>
#include <Engine/Core/Task.h>
#include <Engine/Core/Assert.h>
#include <Engine/Render/ImGuiRenderer.h>
#include <Engine/RHI/RHIBuffer.h>
#include <Engine/RHI/RHITexture.h>

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
		, m_size()
		, m_cmdList(nullptr)
		, m_current(0)
		, m_uploaders()
		, m_textures()
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

		/*Renderer::getInstance().push([this]() {
			setViewData(m_view->getViewData());
			});

		auto id = m_textures[m_current]->getId();
		if (id > 0)
		{
			ImGui::Image((ImTextureID)(intptr_t)id, ImVec2(viewSize.width, viewSize.height), ImVec2(0, 1), ImVec2(1, 0));
		}
		else
		{
			ImGui::Text("No texture available");
		}*/

		ImGui::End();
		ImGui::PopStyleVar();
	}

	void ViewportWidget::onTopWidgetChanged(Widget* old, Widget* current)
	{
		if (old)
		{
			if (auto window = dynamic_cast<Window*>(old))
			{
				window->AttachStateChanged.unbind(this);

				if (m_view)
				{
					onWindowAttachStateChanged(window, false);
				}
			}
		}

		auto window = dynamic_cast<Window*>(current);
		if (!window)
		{
			return;
		}
		window->AttachStateChanged.bindObject(this, &ViewportWidget::onWindowAttachStateChanged);
		if (window->getImGuiRenderer())
		{
			onWindowAttachStateChanged(window, true);
		}
	}

	void ViewportWidget::viewSizeChanged(Size size)
	{
		if (m_view)
		{
			m_size = size;
			//recreateUploaders();
			Renderer::getInstance().push([view=m_view, size]() {
				view->resize(size.width, size.height);
				});
		}
	}

	void ViewportWidget::onWindowAttachStateChanged(Window* window, bool attached)
	{
		v_check(attached == (m_view==nullptr))
		if (attached)
		{
			auto view = std::make_unique<View>();
			m_view = view.get();
			CreateViewTask task = CreateViewTask(std::move(view), m_size);
			Renderer::getInstance().push(createTask(std::move(task)));
			m_cmdList = window->getImGuiRenderer()->getCommandList();
			//recreateUploaders();
		}
		else
		{
			Renderer::getInstance().push([view = m_view]() {
				Renderer::getInstance().removeView(view);
				});
			m_view = nullptr;
			m_cmdList = nullptr;
			clearUploaders();
		}
	}

	void ViewportWidget::recreateUploaders()
	{
		if (m_size.width < 8 || m_size.height < 8 || !m_cmdList)
		{
			return;
		}

		clearUploaders();

		auto size = m_size.width * m_size.height * 3;
		RHITextureDesc desc;
		desc.pixelFormat = Texture::EPixelFormat::R8G8B8;
		desc.size = m_size;
		desc.texClass = RHITextureDesc::Texture2D;
		for (int i = 0; i < 2; ++i)
		{
			auto uploader = std::make_unique<RHIWritePixelBuffer>(size, RHIBuffer::StreamWrite);
			uploader->init(m_cmdList);
			m_uploaders.push_back(std::move(uploader));

			auto texture = std::make_unique<RHITexture2D>(desc);
			texture->init(m_cmdList);
			m_textures.push_back(std::move(texture));
		}

		m_current = 0;
	}

	void ViewportWidget::clearUploaders()
	{
		for (auto& uploader : m_uploaders)
		{
			m_cmdList->deleteResource(uploader.get());
		}

		for (auto& texture : m_textures)
		{
			m_cmdList->deleteResource(texture.get());
		}

		m_textures.clear();
		m_uploaders.clear();
	}

	void ViewportWidget::setViewData(Texture::TextureData& data)
	{
		if (!m_view)
		{
			return;
		}
		if (data.size.width != m_size.width || data.size.height != m_size.height)
		{
			/*V_LOG_ERROR(Editor, "ViewportWidget::setViewData size mismatch: expected {}x{}, got {}x{}",
				m_size.width, m_size.height, data.size.width, data.size.height);*/
			return;
		}

		if (m_uploaders.empty())
		{
			//recreateUploaders();
		}

		auto& currentUploader = m_uploaders[m_current];
		currentUploader->startWrite(m_cmdList, std::move(data.data));
		m_current = (m_current + 1) % m_uploaders.size();
		m_uploaders[m_current]->writeTo(m_textures[m_current].get(), m_cmdList);
	}
}