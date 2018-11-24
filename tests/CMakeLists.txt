cmake_minimum_required (VERSION 3.2)
project (libcpputil-tests)

set(linux-sources
	tuple_for_each.cxx
	tuple_at.cxx
	call_seq.cxx
	make_ref_tuple.cxx
	argv.cxx
	sizeof_tuple.cxx
	marshall.cxx
)

set(osless-sources
	uninitialized_array.cxx
	tuple.cxx
	stack.cxx
	bitmap_allocator.cxx
	queue.cxx
	format.cxx
	meta_compile_without_stdcpp.cxx
	array.cxx
	for_each_param.cxx
	bitview.cxx
	int_type.cxx
	named_type.cxx
	stack_allocator.cxx
	static_config.cxx
	tests_point.cxx
)

set(requires-exceptions-sources
	curl.cxx
)

set(test-sources ${osless-sources})

if (TOOLBOX_HAS_EXCEPTIONS)
	set(test-source "${test-sources} ${requires-exceptions-sources}")
else ()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions")
endif ()

if (TOOLBOX_HAS_STDLIB)
	set(test-sources ${test-sources} ${linux-sources})
else ()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -nostdinc++")
endif ()

add_executable(tests ${test-sources} main.cxx)
target_link_libraries(tests curl cpptoolbox)

include(CTest)
add_test(
    NAME "unit tests"
    COMMAND tests
)
