#ifndef __volucris_size_h__
#define __volucris_size_h__

namespace volucris
{
	struct Size
	{
		int width = 0;
		int height = 0;

		Size() = default;

		Size(int w, int h) : width(w)
			, height(h)
		{
		}

		bool isValid() const { return width > 0 && height > 0; }
	};
}

#endif // !__volucris_size_h__
