#ifndef __volucris_window_h__
#define __volucris_window_h__

#include <string>
#include <memory>
#include "Engine/Core/event.h"

DECLARE_EVENT(OnClose)

namespace volucris
{
	class Window
	{
	public:
		OnClose onClose;

	public:
		Window();

		~Window();

		void pollEvents();

		bool initialize();

		void destroy();

		void* getHandle() const;

	private:
		struct Impl;
		Impl* m_impl;
		int m_width;
		int m_height;
		std::string m_title;
	};
}

#endif // !__volucris_window_h__
