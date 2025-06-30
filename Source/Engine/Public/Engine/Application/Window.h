#ifndef __volucris_window_h__
#define __volucris_window_h__

#include <Engine/Application/Widget.h>
#include <string>
#include <Engine/Core/Delegate.h>

struct GLFWwindow;

namespace volucris
{
	DECLARE_EVENT_MUTI_DELEGATE(OnWindowAttachStateChanged, void, class Window*, bool)

	class ImGuiRenderer;

	class Window : public Widget
	{
	public:
		OnWindowAttachStateChanged AttachStateChanged;

	public:
		Window();

		~Window() override;

		void setTitle(const std::string& title);

		bool isValid() const
		{
			return m_imguiRenderer != nullptr;
		}

		ImGuiRenderer* getImGuiRenderer() const
		{
			return m_imguiRenderer.get();
		}

		GLFWwindow* getHandle() const { return m_handle; }

	protected:
		void onBuild() override;

	private:
		friend class Application;
		friend class Renderer;
		friend class RHICommandList;

		void create(bool offscreen = false);

		void destroy();

		void destroyImGuiRenderer();

	private:
		GLFWwindow* m_handle;
		std::unique_ptr<ImGuiRenderer> m_imguiRenderer;
		std::string m_title;
	};
}

#endif // !__volucris_window_h__
