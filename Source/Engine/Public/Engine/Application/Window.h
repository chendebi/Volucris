#ifndef __volucris_window_h__
#define __volucris_window_h__

#include <Engine/Application/Widget.h>
#include <string>

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

	protected:
		void onBuild() override;

	private:
		friend class Application;

		void create();

		void destroy();

		void destroyImGuiRenderer();

	private:
		GLFWwindow* m_handle;
		std::unique_ptr<ImGuiRenderer> m_imguiRenderer;
		std::string m_title;
	};
}

#endif // !__volucris_window_h__
