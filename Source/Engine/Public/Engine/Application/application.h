#ifndef __volucris_application_h__
#define __volucris_application_h__

#include "Engine/Core/string_help.h"
#include "Engine/Core/types_help.h"
#include <memory>
#include <vector>

namespace volucris
{
	class Window;
	class Context;

	class Application
	{
		static Application* Inst;

	public:
		struct Config
		{
			std::string appName;
			std::vector<std::string> arguments;
			Config(int argc, char* argv[]);
		};

	public:
		Application(const Config& config);

		static Application* Instance()
		{
			return Inst;
		}

		const Config& getAppConfig() const { return m_config; }

		bool initialize();

		void shotdown();

		bool setWindow(const std::shared_ptr<Window>& window);

		bool isInitialized() const { return m_initialized; }

		bool isRunning() const { return m_running; }

		Window* getWindow() const { return m_window.get(); }

		void quit();

		void exec();

	private:
		Config m_config;
		uint8 m_initialized;
		uint8 m_running;
		std::shared_ptr<Window> m_window;
		std::shared_ptr<Context> m_context;
	};
}

extern std::shared_ptr<volucris::Application> volucrisMain(int argc, char* argv[]);

#endif // !__volucris_application_h__
