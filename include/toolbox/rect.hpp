#ifndef _TOOLBOX_RECT_HPP_
#define _TOOLBOX_RECT_HPP_

#include <toolbox/point.hpp>

namespace toolbox
{

template<class T>
struct rect
{
	point<T> top_left, bottom_right;

	constexpr T width() const
	{
		return bottom_right.x - top_left.x;
	}

	constexpr T height() const
	{
		return bottom_right.y - top_left.y;
	}

	void set_width(T width)
	{
		bottom_right.x = top_left.x + width;
	}

	void set_height(T height)
	{
		bottom_right.y = top_left.y + height;
	}
};

} // namespace toolbox

#endif // _TOOLBOX_RECT_HPP_
