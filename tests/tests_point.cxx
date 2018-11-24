#include <toolbox/point.hpp>
#include <toolbox/testing/test.h>

TEST_CASE(point_set_get)
{
	auto p = toolbox::point<int>{1, 2};
	CHECK(p.x == 1);
	CHECK(p.y == 2);
}
