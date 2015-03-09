//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_PROVIDERS_STACK_OVER_HEAP_HPP
#define BOOST_DI_PROVIDERS_STACK_OVER_HEAP_HPP

#include "boost/di/type_traits/ctor_traits.hpp"
#include "boost/di/type_traits/memory_traits.hpp"

namespace boost { namespace di { namespace providers {

template<class>
struct polymorphic_type {
    struct is_not_bound { };
};

#pragma GCC diagnostic push
#pragma GCC diagnostic error "-Wundefined-inline"

template<class T, class = void>
constexpr T* error(...);

#pragma GCC diagnostic pop

class stack_over_heap {
public:
    template<class, class T, class... TArgs>
    auto get(const type_traits::direct&
           , const type_traits::heap&
           , TArgs&&... args) {
        return new T(std::forward<TArgs>(args)...);
    }

    template<class, class T, class... TArgs, std::enable_if_t<aux::is_braces_constructible<T, TArgs...>{}, int> = 0>
    auto get(const type_traits::uniform&
           , const type_traits::heap&
           , TArgs&&... args) {
        return new T{std::forward<TArgs>(args)...};
    }


    template<class, class T, class... TArgs, std::enable_if_t<!aux::is_braces_constructible<T, TArgs...>{}, int> = 0>
    auto get(const type_traits::uniform&
           , const type_traits::heap&
           , TArgs&&... args) {
        return error<T, typename polymorphic_type<T>::is_not_bound>("did you forget to add: 'bind<interface, implementation>'?");
    }

    template<class, class T, class... TArgs>
    auto get(const type_traits::direct&
           , const type_traits::stack&
           , TArgs&&... args) const noexcept {
        return T(std::forward<TArgs>(args)...);
    }

    template<class, class T, class... TArgs>
    auto get(const type_traits::uniform&
           , const type_traits::stack&
           , TArgs&&... args) const noexcept {
        return T{std::forward<TArgs>(args)...};
    }
};

}}} // boost::di::providers

#endif

