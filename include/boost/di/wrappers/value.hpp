//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_WRAPPERS_VALUE_HPP
#define BOOST_DI_WRAPPERS_VALUE_HPP

#include "boost/di/aux_/memory.hpp"

namespace boost { namespace di { namespace wrappers {

template<class T>
class value {
public:
    value(const T& value) noexcept // non explicit
        : value_(value)
    { }

    inline T operator()(const aux::type<T>&) const noexcept {
        return value_;
    }

    inline T* operator()(const aux::type<T*>&) const noexcept {
        return new T(value_);
    }

    inline const T*
    operator()(const aux::type<const T*>&) const noexcept {
        return new T(value_);
    }

    inline T&& operator()(const aux::type<T&&>&) noexcept {
        return std::move(value_);
    }

    template<class I>
    inline aux::shared_ptr<I>
    operator()(const aux::type<aux::shared_ptr<I>>&) const noexcept {
        return aux::shared_ptr<I>(new I(value_));
    }

    template<class I>
    inline aux_::shared_ptr<I>
    operator()(const aux::type<aux_::shared_ptr<I>>&) const noexcept {
        return aux_::shared_ptr<I>(new I(value_));
    }

    template<class I>
    inline aux::unique_ptr<I>
    operator()(const aux::type<aux::unique_ptr<I>>&) const noexcept {
        return aux::unique_ptr<I>(new I(value_));
    }

private:
    T value_;
};

}}} // namespace boost::di::wrappers

#endif

