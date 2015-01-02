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

class stack_over_heap {
public:
    template<class T, class... TArgs>
    auto* get(const type_traits::direct&
            , const type_traits::heap&
            , TArgs&&... args) {
        return new T(std::forward<TArgs>(args)...);
    }

    template<class T, class... TArgs>
    auto* get(const type_traits::uniform&
            , const type_traits::heap&
            , TArgs&&... args) const {
        return new T{std::forward<TArgs>(args)...};
    }

    template<class T, class... TArgs>
    auto get(const type_traits::direct&
           , const type_traits::stack&
           , TArgs&&... args) const noexcept {
        return T(std::forward<TArgs>(args)...);
    }

    template<class T, class... TArgs>
    auto get(const type_traits::uniform&
           , const type_traits::stack&
           , TArgs&&... args) const noexcept {
        return T{std::forward<TArgs>(args)...};
    }
};

}}} // boost::di::providers

#endif

