//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONVERTIBLES_VALUE_HPP
#define BOOST_DI_CONVERTIBLES_VALUE_HPP

#include "boost/di/named.hpp"

#include <utility>
#include <boost/type.hpp>
#include <boost/config.hpp>

namespace boost {
namespace di {
namespace convertibles {

template<typename T>
class value
{
public:
    explicit value(T value)
        : value_(value)
    { }

    template<typename I>
    I convert(const type<I>&) const {
        return value_;
    }

#if defined(BOOST_HAS_RVALUE_REFERENCES)
    template<typename I>
    I&& convert(const type<I&&>&) const {
        return std::move(value_);
    }
#endif

    template<typename I, typename TName>
    named<I, TName> convert(const type<const named<I, TName>&>&) const {
        return value_;
    }

private:
    T value_;
};

} // namespace convertibles
} // namespace di
} // namespace boost

#endif

