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

		void setSize(const Size& size) { width = size.width; height = size.height; }

		Point getPoint() const { return { x, y }; }

		Size getSize() const { return { width, height }; }

		bool isValid() const { return width > 0 && height > 0; }

		bool operator==(const Rect& other) const
		{
			return x == other.x && y == other.y && width == other.width && height == other.height;
		}

		bool operator!=(const Rect& other) const
		{
			return !operator==(other);
		}

		template <class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& x;
			ar& y;
			ar& width;
			ar& height;
		}
	};
}

#endif // !__volucris_rect_h__
