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
	class Renderer;
	class Widget;
	class Viewport;
	class Scene;

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

		void shutdown();

		bool setWindow(const std::shared_ptr<Window>& window);

		bool setRenderer(const std::shared_ptr<Renderer>& renderer);

		void setMainWidget(const std::shared_ptr<Widget>& widget);

		void setScene(const std::shared_ptr<Scene>& scene);

		bool addViewport(const std::shared_ptr<Viewport>& viewport);

		bool isInitialized() const { return m_initialized; }

		bool isRunning() const { return m_running; }

		Window* getWindow();

		Context* getContext() const;

		Renderer* getRenderer() const { return m_renderer.get(); }

		Scene* getScene();

		void quit();

		void exec();

	private:
		Config m_config;
		uint8 m_initialized;
		uint8 m_running;
		std::shared_ptr<Window> m_window;
		std::shared_ptr<Renderer> m_renderer;
		std::shared_ptr<Widget> m_mainWidget;
		std::shared_ptr<Scene> m_scene;
		std::vector<std::shared_ptr<Viewport>> m_viewports;
	};
}

extern std::shared_ptr<volucris::Application> volucrisMain(int argc, char* argv[]);

#define gApp volucris::Application::Instance()

#endif // !__volucris_application_h__
