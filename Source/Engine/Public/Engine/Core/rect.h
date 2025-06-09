#ifndef __volucris_rect_h__
#define __volucris_rect_h__

#include <string>

namespace volucris
{
	struct Point2D
	{
		int x = -1;
		int y = -1;

		bool isValid() const
		{
			return x >= 0 && y >= 0;
		}

		Point2D operator-(const Point2D& pos)
		{
			return { x - pos.x, y - pos.y };
		}
	};

	struct Rect
	{
		int x = 0;
		int y = 0;
		int w = 0;
		int h = 0;

		Rect() = default;

		Rect(int px, int py, int width, int height)
			: x(px), y(py), w(width), h(height)
		{ }

		void setPos(int x, int y)
		{
			this->x = x;
			this->y = y;
		}

		void setSize(int w, int h)
		{
			this->w = w;
			this->h = h;
		}

		bool operator==(const Rect& rect) const
		{
			return x == rect.x && y == rect.y && w == rect.w && h == rect.h;
		}

		bool operator!=(const Rect& rect) const
		{
			return !operator==(rect);
		}

		bool isValid() const { return w > 0 && h > 0; }

		bool contains(int px, int py) const
		{
			return px >= x && px <= x + w &&
				py >= y && py <= y + h;
		}

		Point2D getRelativePoint(int px, int py)
		{
			Point2D pos;
			pos.x = px - x;
			pos.y = py - y;
			return pos;
		}

		Point2D getRelativePoint(const Point2D& pos)
		{
			return getRelativePoint(pos.x, pos.y);
		}

		std::string toString() const
		{
			return "Rect(" + std::to_string(x) + ", " + std::to_string(y) + ", " +
				std::to_string(w) + ", " + std::to_string(h) + ")";
		}
	};

	struct RectF
	{
		float x = 0;
		float y = 0;
		float w = -1;
		float h = -1;

		RectF() = default;

		RectF(float rx, float ry, float rw, float rh)
			: x(rx)
			, y(ry)
			, w(rw)
			, h(rh)
		{
		}

		void setPos(float x, float y)
		{
			this->x = x;
			this->y = y;
		}

		void setSize(float w, float h)
		{
			this->w = w;
			this->h = h;
		}

		bool isValid() const { return w > 0.0f && h > 0.0f; }
	};
}

#endif // !__volucris_rect_h__
