#ifndef __volucris_window_h__
#define __volucris_window_h__

#include <string>
#include <memory>
#include "Engine/Core/event.h"
#include "Engine/Core/size.h"

DECLARE_EVENT(OnClose)
DECLARE_EVENT(OnSizeChanged, int, int)

using OnMouseMove = OnSizeChanged;

namespace volucris
{
	class Window
	{
	public:
		OnClose Close;
		OnSizeChanged FrameSizeChanged;
		OnSizeChanged WindowSizeChanged;
		OnMouseMove MouseMove;

	public:
		Window();

		~Window();

		void pollEvents();

		void setTitle(const std::string& title);

		void setSize(int width, int height);

		bool initialize();

		void destroy();

		void* getHandle() const;

		bool setupImGUI(const std::string& glslversion);

		Size getFrameSize() const { return m_frameSize; }

	private:
		struct Impl;
		Impl* m_impl;
		Size m_size;
		Size m_frameSize;
		std::string m_title;
	};
}

#endif // !__volucris_window_h__
