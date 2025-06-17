#ifndef __volucris_point_h__
#define __volucris_point_h__

#include <fmt/format.h>

namespace volucris
{
	struct Point
	{
		int x;
		int y;

		Point() : x(-1), y(-1) {}

		Point(int px, int py) : x(px), y(py) {}

		bool isValid() const { return x >= 0 && y >= 0; }

		Point operator+(const Point& p) const
		{
			return { x + p.x, y + p.y };
		}

		Point operator-(const Point& p) const
		{
			return { x - p.x, y - p.y };
		}

		Point& operator+=(const Point& p)
		{
			x += p.x;
			y += p.y;
			return *this;
		}

		Point& operator-=(const Point& p)
		{
			x -= p.x;
			y -= p.y;
			return *this;
		}
	};
}

namespace fmt
{
	template <>
	struct formatter<volucris::Point> {
		constexpr auto parse(format_parse_context& ctx) {
			return ctx.begin();
		}

		auto format(const volucris::Point& p, format_context& ctx) const {
			auto out = ctx.out();
			return fmt::format_to(out, "[ {}, {} ]", p.x, p.y);
		}
	};
}

#endif // !__volucris_point_h__
