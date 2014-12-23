//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CORE_POLICY_HPP
#define BOOST_DI_CORE_POLICY_HPP

#include "boost/di/core/binder.hpp"

namespace boost { namespace di { namespace core {

template<
    class TParent
  , class T
  , class TName
  , class TDependency
  , class TDeps
> struct policy {
    struct when {
        using type = T;
        using parent = TParent;
        using name = TName;
    };

    struct then {
        using scope = typename TDependency::scope;
        using expected = typename TDependency::expected;
        using given = typename TDependency::given;
        using name = typename TDependency::name;
    };

    template<class TT, class Name, class TDefault>
    using resolve = decltype(binder::resolve<TT, Name, TDefault>((TDeps*)nullptr))
};

template<
    class TParent
  , class T
  , class TName
  , class TDependency
  , class... TPolicies
> void call(const pool<aux::type_list<TPolicies...>>& policies, TDependency&& dependency) const noexcept {
    void(call_impl<TPolicies, TParent, T, TName>(
        policies, std::forward<TDependency>(dependency))...
    );
}

template<
    class TPolicy
  , class TParent
  , class T
  , class TName
  , class TPolicies
  , class TDependency
> void call_impl(const TPolicies& policies
                        , TDependency&& dependency) const noexcept {
    auto&& injections = data<TParent, T, TName, TDependency, pool_t>{
        std::forward<TDependency>(dependency)
      , (injector&)*this
    };
    (static_cast<const TPolicy&>(policies))(injections);
}

}}} // boost::di::core

#endif

