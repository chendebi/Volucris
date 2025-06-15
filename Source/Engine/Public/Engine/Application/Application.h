#ifndef __volucris_application_h__
#define __volucris_application_h__

#include <Engine/Core/Object.h>
#include <vector>

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

		void addWindow(const std::shared_ptr<Window>& window);

		void removeWindow(const std::shared_ptr<Window>& window);

		void setMainWindow(const std::shared_ptr<Window>& window);

		int exec();

	private:
		static Application* s_instance;
		std::shared_ptr<Window> m_mainWindow;
		std::vector<std::shared_ptr<Window>> m_windows;
	};
}

extern std::shared_ptr<volucris::Application> volucrisEntry(int argc, char** argv);

#define gApp volucris::Application::getInstance()

#endif // !__volucris_application_h__
