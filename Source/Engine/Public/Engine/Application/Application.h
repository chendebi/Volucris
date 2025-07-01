#ifndef __volucris_application_h__
#define __volucris_application_h__

#include <Engine/Core/Object.h>
#include <vector>
#include <functional>
#include <Engine/Core/CircleQueue.h>

namespace volucris
{
	class Window;

	class Application : public Object
	{
	public:
		Application();

		static Application* getInstance()
		{
			return s_instance;
		}

		void pushCommand(const std::function<void()>& command)
		{
			m_queue.push(command);
		}

		void addWindow(const std::shared_ptr<Window>& window);

		void removeWindow(const std::shared_ptr<Window>& window);

		void setMainWindow(const std::shared_ptr<Window>& window);

		void setFocusedWindow(Window* window);

		int getWindowCount() const { return m_windows.size(); }

		int exec();

	private:
		static Application* s_instance;
		Window* m_focusedWindow;
		std::shared_ptr<Window> m_mainWindow;
		std::vector<std::shared_ptr<Window>> m_windows;
		CircleQueue<std::function<void()>> m_queue;
	};
}

extern std::shared_ptr<volucris::Application> volucrisEntry(int argc, char** argv);

#define gApp volucris::Application::getInstance()

#endif // !__volucris_application_h__
