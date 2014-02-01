//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONVERTIBLES_VALUE_HPP
#define BOOST_DI_CONVERTIBLES_VALUE_HPP

#include "boost/di/named.hpp"
#include "boost/di/aux_/memory.hpp"

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
    value(T value) // non explicit
        : value_(value)
    { }

    template<typename I>
    I operator()(const type<I>&) const {
        return value_;
    }

    template<typename I, typename TName>
    I operator()(const type<named<I, TName>&>&) const {
        return value_;
    }


    template<typename I>
    aux::shared_ptr<I> operator()(const type<aux::shared_ptr<I> >&) {
        return aux::shared_ptr<I>(new I(value_));
    }

    template<typename I>
    const aux::shared_ptr<I>& operator()(const type<const aux::shared_ptr<I>& >&) {
        auto* f = new aux::shared_ptr<I>(new I(value_));
        return *f;
    }

    template<typename I>
    I* operator()(const type<I*>&) {
        return new I(value_);
    }

#if defined(BOOST_HAS_RVALUE_REFERENCES)
    template<typename I>
    I&& operator()(const type<I&&>&) {
        return std::move(value_);
    }
#endif

    template<typename I>
    operator I() const {
        return (*this)(type<I>());
    }

private:
    T value_;
};

} // namespace convertibles
} // namespace di
} // namespace boost

#endif

