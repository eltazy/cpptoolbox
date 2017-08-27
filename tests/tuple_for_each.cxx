#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <cpp/tuple_for_each.hpp>

TEST_CASE("Iterate and sum", "[tuple_for_each][tuple]")
{
	auto sum = 0;

	cpp::tuple_for_each(std::make_tuple(1, 2, 3), [&sum](auto arg) { sum += arg; });

	REQUIRE(sum == 6);
}

TEST_CASE("Increment elements", "[tuple_for_each][tuple]")
{
	auto tuple = std::make_tuple(1, 2, 3);

	cpp::tuple_for_each(tuple, [](auto &arg) { arg ++; });

	REQUIRE(std::get<0>(tuple) == 2);
	REQUIRE(std::get<1>(tuple) == 3);
	REQUIRE(std::get<2>(tuple) == 4);
}

struct adder
{
	template<typename T>
	void operator()(const T& arg)
	{
		sum += static_cast<int>(arg);
	}

	int sum = 0;
};

TEST_CASE("Functor with state", "[tuple_for_each][tuple]")
{
	auto fun = cpp::tuple_for_each(std::make_tuple(1, 2, 3), adder{});

	REQUIRE(fun.sum == 6);
}

struct int_doubler
{
	template<typename T, typename U, typename R = void>
	using enable_for_type_t = typename std::enable_if<std::is_same<T, U>::value>::type;
	template<typename T, typename U, typename R = void>
	using disable_for_type_t = typename std::enable_if<!std::is_same<T, U>::value>::type;

	template<typename T>
	enable_for_type_t<T, int> operator()(T &arg)
	{
		arg *= 2;
	}

	template<typename T>
	disable_for_type_t<T, int> operator()(T &) {}
};

TEST_CASE("Selective modification", "[tuple_for_each][tuple]")
{
	auto tuple = std::make_tuple(1, 2, 3.0, 4.0, 5.f, 6.f);
	const auto orig = tuple;

	cpp::tuple_for_each(tuple, int_doubler{});

	REQUIRE(std::get<0>(tuple) == 2);
	REQUIRE(std::get<1>(tuple) == 4);
	REQUIRE(std::get<2>(tuple) == 3.0);
	REQUIRE(std::get<3>(tuple) == 4.0);
	REQUIRE(std::get<4>(tuple) == 5.f);
	REQUIRE(std::get<5>(tuple) == 6.f);
}
