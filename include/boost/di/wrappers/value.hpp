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

    inline operator T() const noexcept {
        return value_;
    }

    inline operator T*() const noexcept {
        return new T{value_}; // transfer ownership
    }

    inline operator const T*() const noexcept {
        return new T{value_}; // transfer ownership
    }

    inline operator T&&() noexcept {
        return std::move(value_);
    }

    template<class I>
    inline operator aux::shared_ptr<I>() const noexcept {
        return new I{value_};
    }

    template<class I>
    inline operator aux_::shared_ptr<I>() const noexcept {
        return new I{value_};
    }

    template<class I>
    inline operator aux::unique_ptr<I>() const noexcept {
        return new I{value_};
    }

private:
    T value_;
};

}}} // namespace boost::di::wrappers

#endif

