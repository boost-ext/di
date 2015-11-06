//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_PROVIDERS_HEAP_HPP
#define BOOST_DI_PROVIDERS_HEAP_HPP

#include "boost/di/type_traits/ctor_traits.hpp"
#include "boost/di/concepts/creatable.hpp"

namespace providers {

class heap {
public:
    template<class TInitialization, class TMemory, class T, class... TArgs>
    struct is_creatable {
        static constexpr auto value =
            concepts::creatable<TInitialization, T, TArgs...>::value;
    };

    template<class, class T, class TMemory, class... TArgs>
    auto get(const type_traits::direct&
           , const TMemory&
           , TArgs&&... args) const {
        return new T(static_cast<TArgs&&>(args)...);
    }

    template<class, class T, class TMemory, class... TArgs>
    auto get(const type_traits::uniform&
           , const TMemory&
           , TArgs&&... args) const {
        return new T{static_cast<TArgs&&>(args)...};
    }
};

} // providers

#endif

