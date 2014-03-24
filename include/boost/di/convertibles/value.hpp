//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONVERTIBLES_VALUE_HPP
#define BOOST_DI_CONVERTIBLES_VALUE_HPP

#include "boost/di/aux_/config.hpp"
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

    template<typename I>
    I& operator()(const type<I&>&) const {
        return value_;
    }

    template<typename I>
    aux::shared_ptr<I> operator()(const type<aux::shared_ptr<I> >&) {
        return aux::shared_ptr<I>(new I(value_));
    }

    template<typename I>
    aux_::shared_ptr<I> operator()(const type<aux_::shared_ptr<I> >&) {
        return aux_::shared_ptr<I>(new I(value_));
    }

    template<typename I>
    const aux::shared_ptr<I>& operator()(const type<const aux::shared_ptr<I>& >&) {
        ref_ = aux::shared_ptr<I>(new I(value_));
        return ref_;
    }

    template<typename I>
    const aux_::shared_ptr<I>& operator()(const type<const aux_::shared_ptr<I>& >&) {
        ref__ = aux_::shared_ptr<I>(new I(value_));
        return ref__;
    }

    template<typename I>
    I* operator()(const type<I*>&) {
        return new I(value_);
    }

    BOOST_DI_FEATURE(1, RVALUE_REFERENCES)(
        template<typename I>
        I&& operator()(const type<I&&>&) {
            return std::move(value_);
        }
    )

private:
    mutable aux::shared_ptr<T> ref_;
    mutable aux_::shared_ptr<T> ref__;
    mutable T value_;
};

} // namespace convertibles
} // namespace di
} // namespace boost

#endif

