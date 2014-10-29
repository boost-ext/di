//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_SCOPES_UNIQUE_HPP
#define BOOST_DI_SCOPES_UNIQUE_HPP

#include "boost/di/wrappers/copy.hpp"

#include <boost/function.hpp>
#include <boost/mpl/int.hpp>

namespace boost {
namespace di {
namespace scopes {

template<template<typename> class TWrapper = wrappers::copy>
class unique
{
public:
    typedef mpl::int_<0> priority;

    template<typename TExpected, typename>
    class scope
    {
    public:
        typedef TWrapper<TExpected> result_type;

        result_type create(const function<TExpected*()>& f) {
            return f;
        }
    };
};

} // namespace scopes
} // namespace di
} // namespace boost

#endif

