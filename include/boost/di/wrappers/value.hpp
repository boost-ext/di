//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_WRAPPERS_VALUE_HPP
#define BOOST_DI_WRAPPERS_VALUE_HPP

#include "boost/di/aux_/config.hpp"
#include "boost/di/aux_/memory.hpp"

#include <utility>
#include <boost/type.hpp>
#include <boost/config.hpp>

namespace boost {
namespace di {
namespace wrappers {

template<typename T>
class value
{
public:
    value(const T& value) // non explicit
        : value_(value)
    { }

    T operator()(const type<T>&) const {
        return value_;
    }

    T* operator()(const type<T*>&) const {
        return new T(value_);
    }

    const T* operator()(const type<const T*>&) const {
        return new T(value_);
    }

    BOOST_DI_FEATURE(RVALUE_REFERENCES)(
        T&& operator()(const type<T&&>&) {
            return std::move(value_);
        }
    )

    template<typename I>
    aux::shared_ptr<I> operator()(const type<aux::shared_ptr<I> >&) const {
        return aux::shared_ptr<I>(new I(value_));
    }

    template<typename I>
    aux_::shared_ptr<I> operator()(const type<aux_::shared_ptr<I> >&) const {
        return aux_::shared_ptr<I>(new I(value_));
    }

    template<typename I>
    aux::auto_ptr<I> operator()(const type<aux::auto_ptr<I> >&) const {
        return aux::auto_ptr<I>(new I(value_));
    }

    template<typename I>
    aux::unique_ptr<I> operator()(const type<aux::unique_ptr<I> >&) const {
        return aux::unique_ptr<I>(new I(value_));
    }

private:
    T value_;
};

} // namespace wrappers
} // namespace di
} // namespace boost

#endif

