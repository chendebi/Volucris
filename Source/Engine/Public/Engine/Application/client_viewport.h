#ifndef __volucris_client_viewport_h__
#define __volucris_client_viewport_h__

#include "Engine/Scene/viewport.h"
#include "Engine/Application/window.h"

namespace volucris
{
	class ClientViewport : public Viewport
	{
	public:
		OnMouseMove MouseMove;

	public:
		ClientViewport();

		~ClientViewport();

		void setClientRect(const Rect& rect);

	protected:
		virtual void onFrameSizeChanged(int width, int height);

		virtual void onMouseMoveChanged(int x, int y);

	private:


	private:
		struct Impl;
		Impl* m_impl;
		Rect m_clientRect;
	};
}

#endif // !__volucris_client_viewport_h__
