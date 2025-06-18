#ifndef __volucris_size_h__
#define __volucris_size_h__

#include <fmt/format.h>

namespace volucris
{
	struct Size
	{
		int width;
		int height;

		Size() : width(-1), height(-1) {}

		Size(int pwidth, int pheight) : width(pwidth), height(pheight) {}

		bool isValid() const { return width >= 0 && height >= 0; }

		Size operator+(const Size& size) const
		{
			return { width + size.width, height + size.height };
		}

		Size operator-(const Size& size) const
		{
			return { width - size.width, height - size.height };
		}

		Size& operator+=(const Size& size)
		{
			width += size.width;
			height += size.height;
			return *this;
		}

		Size& operator-=(const Size& size)
		{
			width -= size.width;
			height -= size.height;
			return *this;
		}

		bool operator==(const Size& size) const
		{
			return width == size.width && height == size.height;
		}

		bool operator!=(const Size& size) const
		{
			return !operator==(size);
		}
	};
}

namespace fmt
{
	template <>
	struct formatter<volucris::Size> {
		constexpr auto parse(format_parse_context& ctx) {
			return ctx.begin();
		}

		auto format(const volucris::Size& s, format_context& ctx) const {
			auto out = ctx.out();
			return fmt::format_to(out, "[ {}, {} ]", s.width, s.height);
		}
	};
}

#endif // !__volucris_size_h__
