#ifndef __volucris_window_h__
#define __volucris_window_h__

#include <string>
#include <memory>
#include "Engine/Core/event.h"
#include "Engine/Core/size.h"
#include "Engine/Core/types_help.h"
#include <Engine/Core/rect.h>

DECLARE_EVENT(OnClose)
DECLARE_EVENT(OnSizeChanged, int, int)
DECLARE_EVENT(OnMouseEvent, volucris::MouseEvent)

using OnMouseMove = OnSizeChanged;

namespace volucris
{
	class Window
	{
	public:
		enum Flags
		{
			FRAMELESS = 0x01,
			FIX_SIZE = 0x02,
			FULL_SCREEN = 0x04,
		};

	public:
		OnClose Close;
		OnSizeChanged FrameSizeChanged;
		OnSizeChanged WindowSizeChanged;
		OnMouseMove MouseMove;
		OnMouseEvent MousePressed;
		OnMouseEvent MouseReleased;

	public:
		Window();

		~Window();

		void pollEvents();

		void setTitle(const std::string& title);

		void setPosition(int x, int y);

		void setSize(int width, int height);

		bool initialize();

		void destroy();

		void* getHandle() const;

		bool setupImGUI(const std::string& glslversion);

		Size getFrameSize() const { return m_frameSize; }

		void setFullScreen(bool enabled);

		void setFrameless(bool enabled);

	private:
		struct Impl;
		Impl* m_impl;
		Rect m_rect;
		Rect m_normalRect;
		Size m_frameSize;
		std::string m_title;
		uint8 m_fullScreen;
		uint8 m_frameless;
	};
}

#endif // !__volucris_window_h__
