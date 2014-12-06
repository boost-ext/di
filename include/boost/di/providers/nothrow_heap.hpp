//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_PROVIDERS_NOTHROW_HEAP_HPP
#define BOOST_DI_PROVIDERS_NOTHROW_HEAP_HPP

#include <new>
#include "boost/di/type_traits/ctor_traits.hpp"

namespace boost { namespace di { namespace providers {

class nothrow_heap {
public:
    template<class T, class TMemory, class... TArgs>
    auto* get(const type_traits::direct&
            , const TMemory&
            , TArgs&&... args) const noexcept {
        return new (std::nothrow) T(std::forward<TArgs>(args)...);
    }

    template<class T, class TMemory, class... TArgs>
    auto* get(const type_traits::aggregate&
            , const TMemory&
            , TArgs&&... args) const noexcept {
        return new (std::nothrow) T{std::forward<TArgs>(args)...};
    }
};

}}} // boost::di::providers

#endif

