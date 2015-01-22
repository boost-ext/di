//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CORE_VISITOR_HPP
#define BOOST_DI_CORE_VISITOR_HPP

#include "boost/di/aux_/utility.hpp"
#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/core/pool.hpp"

namespace boost { namespace di { namespace core {

BOOST_DI_HAS_METHOD(call_operator, operator());

template<class TDeps>
class visitor {
    template<
        class TVisitor
      , class T
      , class TName
      , class TVisitors
      , class TDependency
      , class... TArgs
    > static void call_impl(const TVisitors& visitors, TDependency&& dependency) noexcept {
        struct arg {
            using type = T;
            using name = TName;
        };

        call_impl_args<arg, TDependency, TVisitor, TArgs...>(
            static_cast<const TVisitor&>(visitors), dependency
        );
    }

    template<class TArg, class TDependency, class TVisitor, class... TArgs>
    static std::enable_if_t<has_call_operator<TVisitor, TArg>{}>
    call_impl_args(const TVisitor& visitor, TDependency&&) noexcept {
        (visitor)(TArg{});
    }

    template<class TArg, class TDependency, class TVisitor, class... TArgs>
    static std::enable_if_t<has_call_operator<TVisitor, TArg, TDependency, TArgs...>{}>
    call_impl_args(const TVisitor& visitor, TDependency&& dependency) noexcept {
        (visitor)(TArg{}, dependency, aux::type<TArgs>{}...);
    }

public:
    template<
        class T
      , class TName
      , class TInitialization
      , class TDependency
      , class... TArgs
      , class... TVisitors
    > static void call(const pool<aux::type_list<TVisitors...>>& visitors
                     , TDependency&& dependency
                     , aux::pair<TInitialization, aux::type_list<TArgs...>>) noexcept {
        int _[]{0, (call_impl<TVisitors, T, TName, TVisitors, TDependency, TArgs...>(
            visitors, dependency), 0)...}; (void)_;
    }
};

}}} // boost::di::core

#endif

