#include <toolbox/rect.hpp>
#include <toolbox/testing/test.h>

TEST_CASE(rect_get_width_height)
{
	toolbox::rect<int> r{{0, 0}, {15, 15}};

	CHECK(r.width() == 15);
	CHECK(r.height() == 15);
}

TEST_CASE(rect_resize)
{
	toolbox::rect<int> r{{0, 0}, {10, 10}};

	r.set_width(20);
	r.set_height(20);

	CHECK(r.top_left.x == 0);
	CHECK(r.top_left.y == 0);
	CHECK(r.width() == 20);
	CHECK(r.height() == 20);
}
