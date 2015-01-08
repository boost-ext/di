//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
// // Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CORE_PROVIDER_HPP
#define BOOST_DI_CORE_PROVIDER_HPP

#include "boost/di/aux_/utility.hpp"
#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/type_traits/memory_traits.hpp"

namespace boost { namespace di { namespace core {

BOOST_DI_HAS_TYPE(deleter, template deleter<void>);

template<class...>
struct provider;

template<class, class T, class = std::false_type>
struct get_deleter {
    using type = std::default_delete<T>;
};

template<class TInjector, class T>
struct get_deleter<TInjector, T, std::true_type> {
    using type = typename TInjector::config::template deleter<T>;
};

template<
    class TDependency
  , class TParent
  , class TInjector
  , class TInitialization
  , class... TArgs
> struct provider<
    TDependency
  , TParent
  , aux::pair<TInitialization, aux::type_list<TArgs...>>
  , TInjector
> {
    using deleter = typename get_deleter<
        TInjector
      , typename TDependency::expected
      , typename has_deleter<typename TInjector::config>::type
     >::type;

    template<class TMemory = type_traits::heap>
    auto get(const TMemory& memory = {}) const {
        auto&& config = injector_.config_;
        return config.provider().template get<typename TDependency::given>(
            TInitialization{}
          , memory
          , injector_.template create_t<TParent>(aux::type<TArgs>{})...
        );
    }

    const TInjector& injector_;
};

}}} // boost::di::core

#endif

