#ifndef __volucris_rect_h__
#define __volucris_rect_h__

#include <Engine/Core/Point.h>
#include <Engine/Core/Size.h>

namespace volucris
{
	struct Rect
	{
		int x;
		int y;
		int width;
		int height;

		Rect() : x(-1), y(-1), width(-1), height(-1) {}

		Rect(int rx, int ry, int w, int h) : x(rx), y(ry), width(w), height(h) {}

		void setPoint(const Point& p) { x = p.x; y = p.y; }

		void setSize(int w, int h) { width = w; height = h; }

		Point getPoint() const { return { x, y }; }

		Size getSize() const { return { width, height }; }

		bool isValid() const { return width > 0 && height > 0; }
	};
}

#endif // !__volucris_rect_h__
