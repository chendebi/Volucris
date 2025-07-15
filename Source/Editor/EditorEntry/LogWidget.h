#ifndef __volucris_log_widget_h__
#define __volucris_log_widget_h__

#include <Engine/Application/Widget.h>
#include <Engine/Core/CircleBuffer.h>

namespace volucris
{
	class LogWidget : public Widget
	{
	public:
		LogWidget();

	protected:
		void onBuild() override;

	private:
		void onLogAdded(const std::string& log);

	private:
		CircleBuffer<std::string> m_logs;
	};
}

#endif // !__volucris_log_widget_h__
