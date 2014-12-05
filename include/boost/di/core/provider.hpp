//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
// // Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CORE_PROVIDER_HPP
#define BOOST_DI_CORE_PROVIDER_HPP

#include "boost/di/aux_/utility.hpp"
#include "boost/di/type_traits/memory_traits.hpp"
#include "boost/di/providers/nothrow_reduce_heap_usage.hpp"

namespace boost { namespace di { namespace core {

template<class...>
struct provider;

template<
    class T
  , class TGiven
  , class TInjector
  , class TInitialization
  , class... TArgs
> struct provider<
    T
  , TGiven
  , aux::pair<TInitialization, aux::type_list<TArgs...>>
  , TInjector
> {
    template<class TMemory = type_traits::heap>
    decltype(auto) get(const TMemory& memory = {}) const noexcept {
        return providers::nothrow_reduce_heap_usage{}.get<TGiven>(
            TInitialization{}
          , memory
          , injector_.template create<TArgs, T>()...
        );
    }

    const TInjector& injector_;
};

}}} // namespace boost::di::core

#endif

