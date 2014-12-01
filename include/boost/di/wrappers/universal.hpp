//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_WRAPPERS_UNIVERSAL_HPP
#define BOOST_DI_WRAPPERS_UNIVERSAL_HPP

#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/aux_/memory.hpp"
#include "boost/di/named.hpp"

namespace boost { namespace di { namespace wrappers {

template<class T>
class universal {
public:
    template<class TWrapper>
    explicit universal(const TWrapper& wrapper) noexcept
        : object_(wrapper(aux::type<T>()))
    { }

    inline operator T() const noexcept {
        return object_;
    }

    inline operator T&&() noexcept {
        return std::move(object_);
    }

private:
    T object_;
};

template<class T, class TName>
class universal<named<T, TName>> {
public:
    template<class TWrapper>
    explicit universal(const TWrapper& wrapper) noexcept
        : object_(wrapper(aux::type<T>()))
    { }

    inline operator T() const noexcept {
        return object_;
    }

    inline operator named<T, TName>() const noexcept {
        return object_;
    }

private:
    T object_;
};

}}} // namespace boost::di::wrappers

#endif

