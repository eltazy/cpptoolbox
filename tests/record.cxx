#include "catch.hpp"
#include <iostream>
#include <toolbox/gcc/type_to_string.hpp>
#include <toolbox/record.hpp>

enum class IndexA
{
	Magic,
	Size,
	Value,
	VectorSize,
	Vector
};

using f = toolbox::record::factory<IndexA>;

using blueprintA = f::blueprint<
	f::array<IndexA::Magic                 , char, 2>,
	f::value<IndexA::Size                  , int>,
	f::value<IndexA::Value                 , float>,
	f::vector_size<IndexA::VectorSize      , int, IndexA::Vector>,
	f::vector<IndexA::Vector               , int, IndexA::VectorSize>
>;

TEST_CASE("", "[record]")
{
	f::tuple<blueprintA> t1 = {{'A', 'B'}, 10, 1.234f, 0, {1, 2, 3}};
	std::cout << t1.get_storage<IndexA::VectorSize>() << std::endl;
	std::cout << t1.get<IndexA::VectorSize>() << std::endl;

	t1.serialize([]() {});

/*	std::cout << "types: " << toolbox::gcc::type_to_string(t1) << std::endl;

	t1.for_each([](auto el, auto type)
	{
		std::cout << "argument of type   : " << toolbox::gcc::type_to_string(el) << std::endl;
		std::cout << "     blueprint type: " << toolbox::gcc::type_to_string(type) << std::endl << std::endl;
	});

	std::cout << blueprintA::get<IndexA::Size>(t1) << std::endl;
	std::cout << t1.get<IndexA::Size>() << std::endl;
*/
//	blueprintA::serialize(t1);
}
