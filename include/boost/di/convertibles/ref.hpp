//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONVERTIBLES_REF_HPP
#define BOOST_DI_CONVERTIBLES_REF_HPP

#include "boost/di/named.hpp"

#include <boost/type.hpp>
#include <boost/ref.hpp>

namespace boost {
namespace di {
namespace convertibles {

template<typename T>
class ref
{
public:
    explicit ref(const reference_wrapper<T>& object)
        : object_(object)
    { }

    template<typename I>
    I& operator()(const type<I&>&) const {
        return object_;
    }

    template<typename I, typename TName>
    named<I, TName> operator()(const type<const named<I, TName>&>&) const {
        return named<I, TName>(object_);
    }

private:
    reference_wrapper<T> object_;
};

} // namespace convertibles
} // namespace di
} // namespace boost

#endif

