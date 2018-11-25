#pragma once

#include <toolbox/std/utility.hpp>

namespace toolbox
{
namespace cpp
{

template<
    typename T,
    typename TAG
>
struct named_type
{
    named_type()
        : value_ (T{})
    {}

    explicit named_type(const T& t)
        : value_ (t)
    {
    }

    explicit named_type(T &&t)
        : value_ (std::move(t))
    {
    }

    operator T() const
    {
        return value_;
    }

private:
    T value_;
};

}
}