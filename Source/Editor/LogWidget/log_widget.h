#ifndef __volucris_log_widget_h__
#define __volucris_log_widget_h__

#include <Engine/Application/widget.h>

namespace volucris
{
	class LogBuffer;

	class LogWidget : public Widget
	{
	public:
		LogWidget();

		void build() override;

	private:
	};
}

#endif // !__volucris_log_widget_h__
