//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
// // Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CORE_PROVIDER_HPP
#define BOOST_DI_CORE_PROVIDER_HPP

#include "boost/di/aux_/utility.hpp"
#include "boost/di/type_traits/memory_traits.hpp"

namespace boost { namespace di { namespace core {

template<class...>
struct provider;

template<
    class T
  , class TParent
  , class TInjector
  , class TInitialization
  , class... TArgs
> struct provider<
    T
  , TParent
  , aux::pair<TInitialization, aux::type_list<TArgs...>>
  , TInjector
> {
    template<class TMemory = type_traits::heap>
    auto get(const TMemory& memory = {}) const {
        using config = typename TInjector::config;
        return config{}.provider().template get<T>(
            TInitialization{}
          , memory
          , injector_.template create_t<TParent>(aux::type<TArgs>{})...
        );
    }

    const TInjector& injector_;
};

}}} // boost::di::core

#endif

