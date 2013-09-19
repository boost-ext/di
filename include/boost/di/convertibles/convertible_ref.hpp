//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONVERTIBLES_CONVERTIBLE_REF_HPP
#define BOOST_DI_CONVERTIBLES_CONVERTIBLE_REF_HPP

#include <boost/type_traits/is_arithmetic.hpp>
#include <boost/mpl/if.hpp>

#include "boost/di/named.hpp"

namespace boost {
namespace di {
namespace convertibles {

template<typename T>
class convertible_ref
{
public:
    explicit convertible_ref(T& object)
        : object_(object)
    { }

    convertible_ref(const convertible_ref& copy)
        : object_(copy.object_)
    { }

    operator T&() const {
        return object_;
    }

    template<typename I, typename TName>
    operator named<I, TName>() const {
        return object_;
    }

private:
    typename mpl::if_<is_arithmetic<T>, T, T&>::type object_;
};

template<typename T>
class convertible_ref_const
{
public:
    explicit convertible_ref_const(const T& object)
        : object_(object)
    { }

    convertible_ref_const(const convertible_ref_const& copy)
        : object_(copy.object_)
    { }

    operator const T&() const {
        return object_;
    }

    template<typename I, typename TName>
    operator named<I, TName>() const {
        return object_;
    }

private:
    typename mpl::if_<is_arithmetic<T>, T, const T&>::type object_;
};

} // namespace convertibles
} // namespace di
} // namespace boost

#endif

