#ifndef __volucris_window_h__
#define __volucris_window_h__

#include <Engine/Application/Widget.h>
#include <string>
#include <Engine/Core/Delegate.h>
#include <Engine/Core/TypesHelp.h>

struct GLFWwindow;

namespace volucris
{
	class ImGuiRenderer;

	class Window : public Widget
	{
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

		bool isCurrent() const { return m_focused; }

		std::string getTitle() const { return m_title; }

	protected:
		void onBuild() override;

		void onWindowFocusChanged(FocusEvent* event) override;

	private:
		friend class Application;
		friend class Renderer;
		friend class RHICommandList;

		void setFocused(bool focused);

		void create(bool offscreen = false);

		void destroy();

		void destroyImGuiRenderer();

	private:
		GLFWwindow* m_handle;
		std::unique_ptr<ImGuiRenderer> m_imguiRenderer;
		std::string m_title;
		uint8 m_focused;
	};
}

#endif // !__volucris_window_h__
