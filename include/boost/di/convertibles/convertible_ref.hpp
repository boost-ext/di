//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONVERTIBLES_CONVERTIBLE_REF_HPP
#define BOOST_DI_CONVERTIBLES_CONVERTIBLE_REF_HPP

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
    T& object_;
};

} // namespace convertibles
} // namespace di
} // namespace boost

#endif

