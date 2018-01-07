#ifndef _TOOLBOX_RECORD_HPP_
#define _TOOLBOX_RECORD_HPP_

#include <meta/meta.hpp>
#include <toolbox/cpp/tuple_for_each.hpp>

namespace toolbox
{
namespace record
{

struct value_tag {};
struct vector_tag {};

namespace detail
{

template<typename T, typename ResultT, typename IndexType, IndexType Index>
struct indexable_base
{
	static constexpr IndexType index = Index;
	using base_type = T;
	using type = ResultT;

	template<typename R> const type& get(R& r) const
	{
		return r.template get_storage<Index>();
	}

	template<typename R> void set(R &r, const type& v) const
	{
		r.template get_storage<Index>() = v;
	}
};

template<typename T, typename IndexType, IndexType Index>
struct value : public indexable_base<T, T, IndexType, Index>
{
	using tag = value_tag;
};

template<typename T, std::size_t N, typename IndexType, IndexType Index>
struct array : public indexable_base<T, std::array<T, N>, IndexType, Index>
{
	using tag = vector_tag;
};

template<typename T, typename IndexType, IndexType Index, IndexType SizeIndex>
struct vector : public indexable_base<T, std::vector<T>, IndexType, Index>
{
	using tag = vector_tag;
};

template<typename T, typename IndexType, IndexType Index, IndexType VectorIndex>
struct vector_size : public indexable_base<T, T, IndexType, Index>
{
	using tag = value_tag;

	template<typename R> T get(R &r) const
	{
		return r.template get_storage<VectorIndex>().size();
	}
};

template<typename IndexType, typename... Args>
struct blueprint
{
	using types = meta::list<Args...>;
	using index_type = IndexType;

	template<IndexType Index>
	struct match_index { template<typename T> using invoke = std::integral_constant<bool, T::index == Index>; };

	template<IndexType Index>
	using type_by_index = meta::front<meta::find_if<types, match_index<Index>>>;

	template<IndexType Index>
	using index_of = meta::find_index<types, type_by_index<Index>>;
};

template<typename Blueprint, typename... Args>
struct tuple : public std::tuple<Args...>
{
	using blueprint = Blueprint;
	static constexpr auto size = sizeof...(Args);

	tuple() : std::tuple<Args...>() {}
	tuple(Args&& ... args) : std::tuple<Args...>(args...) {}

	template<typename Blueprint::index_type Index>
	auto& get_storage()
	{
		using tuple_index = typename Blueprint::template index_of<Index>;
		return std::get<tuple_index::value>(*this);
	}

	template<typename Blueprint::index_type Index>
	auto get()
	{
		using type_blueprint = typename Blueprint::template type_by_index<Index>;
		return type_blueprint().get(*this);
	}

	template<typename S>
	struct serializer_impl
	{
		S &s;
		serializer_impl(S &s) : s (s) {}

		template<typename E>
		void operator()(E)
		{
			s();
		}
	};

	template<typename S>
	void serialize(S &&serializer)
	{
		serializer_impl<S> s{serializer};
		meta::for_each(typename Blueprint::types{}, s);
	}
};

} // namespacec detail

/*

template<typename IndexType, typename... Args>
struct blueprint
{
	using types = meta::list<Args...>;
	using index_type = IndexType;
	static constexpr auto size = sizeof...(Args);

	// extract T::type from T
	struct type_extractor { template<typename T> using invoke = typename T::type; };

	// extract list of types from list of {value, optional, etc...}
	using extracted_types = meta::transform<types, type_extractor>;

	// create std::tuple based on blueprint
	using tuple = meta::apply<meta::quote<record::detail::tuple>, meta::push_front<extracted_types, blueprint<IndexType, Args...>>>;

private: // for_each_impl
	template<std::size_t I, typename T, typename F>
	static typename std::enable_if<I == size>::type
	for_each_impl(T &&t, F &&func) {}

	template<std::size_t I, typename T, typename F>
	static typename std::enable_if<I != size>::type
	for_each_impl(T &&t, F &&func) {
		func(std::get<I>(t), meta::at_c<types, I>{});
		for_each_impl<I + 1>(t, std::forward<F>(func));
	}

public:
	template<typename T, typename F>
	static void for_each(T &&t, F &&func)
	{
		for_each_impl<0>(t, std::forward<F>(func));
	}

	template<IndexType Index>
	struct match_index { template<typename T> using invoke = std::integral_constant<bool, T::index == Index>; };

	template<IndexType Index, typename T>
	static auto&
	get(T &&t)
	{
		// find first type in types list which have index Index
		using element_type = meta::front<meta::find_if<types, match_index<Index>>>;

		// now calculate "index" (offset) from the beginning of the list
		using offset = meta::find_index<types, element_type>;

		return element_type();
	}

};

// tuple methods
template<typename Blueprint, typename... Args> template<typename F>
void tuple<Blueprint, Args...>::for_each(F &&func)
{
	blueprint::for_each(*this, func);
}

template<typename Blueprint, typename... Args> template<typename Blueprint::index_type Index>
auto& tuple<Blueprint, Args...>::get()
{
	return blueprint::template get<Index>(*this);
}

} // namespace detail
*/

template<typename IndexType>
struct factory
{
	template<typename... Args>
	using blueprint = detail::blueprint<IndexType, Args...>;

	struct get_type { template<typename T> using invoke = typename T::type; };

	template<typename Blueprint>
	using tuple = meta::apply<
		meta::quote<record::detail::tuple>,
		meta::push_front<meta::transform<typename Blueprint::types, get_type>, Blueprint>
	>;

	template<IndexType index, typename T>
	using value = detail::value<T, IndexType, index>;

	template<IndexType index, typename T, std::size_t N>
	using array = detail::array<T, N, IndexType, index>;

	template<IndexType Index, typename T, IndexType SizeIndex>
	using vector = detail::vector<T, IndexType, Index, SizeIndex>;

	template<IndexType Index, typename T, IndexType VectorIndex>
	using vector_size = detail::vector_size<T, IndexType, Index, VectorIndex>;
};

} // namespace record

} // namespace toolbox

#endif // _TOOLBOX_RECORD_HPP_
