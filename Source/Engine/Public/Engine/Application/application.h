#ifndef __volucris_application_h__
#define __volucris_application_h__

#include "Engine/Core/string_help.h"
#include "Engine/Core/types_help.h"
#include <memory>
#include <vector>
#include "Engine/Core/circle_queue.h"
#include <functional>
#include "Engine/Core/size.h"
#include <Engine/Core/event.h>

namespace volucris
{
	class Window;
	class Context;
	class Renderer;
	class Widget;
	class Level;

	DECLARE_EVENT(OnLevelChanged, std::shared_ptr<Level>)

	struct StatInfo
	{
		double frameTime;
		double frameRate;
	};

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

		OnLevelChanged CurrentLevelChanged;

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

		void setLevel(const std::shared_ptr<Level>& level);

		bool isInitialized() const { return m_initialized; }

		bool isRunning() const { return m_running; }

		Window* getWindow();

		Context* getContext() const;

		Renderer* getRenderer() const { return m_renderer.get(); }

		void quit();

		void exec();

		void pushCommand(const std::function<void()>& command);

		std::shared_ptr<Level> getCurrentLevel() { return m_level; }

		StatInfo getStatInfo() const;

	protected:
		virtual void tick(double delta) {}

	private:
		Config m_config;
		uint8 m_initialized;
		uint8 m_running;
		double m_delta;
		std::shared_ptr<Window> m_window;
		std::shared_ptr<Renderer> m_renderer;
		std::shared_ptr<Widget> m_mainWidget;
		std::shared_ptr<Level> m_level;
		CircleQueue<std::function<void()>> m_queue;
	};
}

extern std::shared_ptr<volucris::Application> volucrisMain(int argc, char* argv[]);

#define gApp volucris::Application::Instance()

#endif // !__volucris_application_h__
