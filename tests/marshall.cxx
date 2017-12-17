#include "catch.hpp"
#include <toolbox/marshall/marshall.hpp>
#include <sstream>

struct fixed_size : public toolbox::marshall::record<
		std::uint32_t,
		std::uint32_t,
		std::uint8_t,
		toolbox::marshall::array<int, int>
	>
{
	fixed_size() : record(
		{"seqNumber", "Sequence number of the message"},
		{"payload", "Value to be written"},
		{"crc", "Error checking value"},
		{"arr", "Array of ints"}
	)
	{}
};

struct compound : public toolbox::marshall::record<
		std::int32_t,
		fixed_size,
		std::uint16_t
	>
{
	compound() : record(
		{"prefix", "a prefix number"},
		{"fixed", "a fixed size object"},
		{"postfix", "a 16 bit postix value"}
	)
	{}
};

TEST_CASE("Binary stringstream serialize-deserialize", "[marshall]")
{
	compound fs, fd;
	std::stringstream buffer;

	fs.get<0>() = 1;
	fs.get<1>().get<0>() = 2;
	fs.get<1>().get<1>() = 3;
	fs.get<1>().get<2>() = 4;
	fs.get<1>().get<3>().push_back(42);
	fs.get<2>() = 5;

	toolbox::marshall::serialize(buffer, fs);
	toolbox::marshall::deserialize(buffer, fd);

	REQUIRE(fs.get<0>() == fd.get<0>());
	REQUIRE(fs.get<1>().get<0>() == fd.get<1>().get<0>());
	REQUIRE(fs.get<1>().get<1>() == fd.get<1>().get<1>());
	REQUIRE(fs.get<1>().get<2>() == fd.get<1>().get<2>());
	REQUIRE(fs.get<1>().get<3>().size() == 1);
	REQUIRE(fs.get<1>().get<3>()[0] == 42);
	REQUIRE(fs.get<2>() == fd.get<2>());
}

TEST_CASE("JSON serialize-deserialize", "[marshall]")
{
	compound fs, fd;

	fs.get<0>() = 1;
	fs.get<1>().get<0>() = 2;
	fs.get<1>().get<1>() = 3;
	fs.get<1>().get<2>() = 4;
	fs.get<1>().get<3>().push_back(42);
	fs.get<1>().get<3>().push_back(43);
	fs.get<2>() = 5;

	auto json = toolbox::marshall::json::serialize(fs);
	toolbox::marshall::json::deserialize(json, fd);

	REQUIRE(fs.get<0>() == fd.get<0>());
	REQUIRE(fs.get<1>().get<0>() == fd.get<1>().get<0>());
	REQUIRE(fs.get<1>().get<1>() == fd.get<1>().get<1>());
	REQUIRE(fs.get<1>().get<2>() == fd.get<1>().get<2>());
	REQUIRE(fs.get<1>().get<3>().size() == 2);
	REQUIRE(fs.get<1>().get<3>()[0] == 42);
	REQUIRE(fs.get<1>().get<3>()[1] == 43);
	REQUIRE(fs.get<2>() == fd.get<2>());
}