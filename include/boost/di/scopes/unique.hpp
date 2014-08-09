//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_SCOPES_UNIQUE_HPP
#define BOOST_DI_SCOPES_UNIQUE_HPP

#include "boost/di/wrappers/copy.hpp"

namespace boost {
namespace di {
namespace scopes {

template<template<typename> class TWrapper = wrappers::copy>
class unique
{
public:
    typedef aux::mpl::int_<0> priority;

    template<typename TExpected>
    class scope
    {
    public:
        typedef TWrapper<TExpected> result_type;

        result_type create(const std::function<TExpected*()>& f) {
            return f;
        }
    };
};

} // namespace scopes
} // namespace di
} // namespace boost

#endif

