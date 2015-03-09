//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CORE_POLICY_HPP
#define BOOST_DI_CORE_POLICY_HPP

#include "boost/di/aux_/utility.hpp"
#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/core/pool.hpp"

namespace boost { namespace di { namespace core {

BOOST_DI_HAS_METHOD(call_operator, operator());
BOOST_DI_HAS_TYPE(compile_time);

template<class T, class TName, class TDeps>
        struct arg_ {
            struct arg {
                using type = T;
            };
            using type BOOST_DI_UNUSED = T;
            using name BOOST_DI_UNUSED = TName;
            using is_root BOOST_DI_UNUSED = std::false_type;

            template<class T_, class TName_, class TDefault_>
            using resolve =
                decltype(core::binder::resolve<T_, TName_, TDefault_>((TDeps*)nullptr));

        };

template<class TDeps>
class policy {
public:
    template<
        class T
      , class TName
      , class TInitialization
      , class TDependency
      , class... TCtor
      , class... TPolicies
    > static void call(const pool<aux::type_list<TPolicies...>>& policies
                     , TDependency&& dependency
                     , aux::pair<TInitialization, aux::type_list<TCtor...>>) noexcept {
        int _[]{0, (call_impl<TPolicies, T, TName, TPolicies, TDependency, TCtor...>(
            policies, dependency), 0)...}; (void)_;
    }

private:
    template<
        class TPolicy
      , class T
      , class TName
      , class TPolicies
      , class TDependency
      , class... TCtor
    > static void call_impl(const TPolicies& policies, TDependency&& dependency) noexcept {

        call_impl_type<arg_<T, TName, TDeps>, TDependency, TPolicy, TCtor...>(
            static_cast<const TPolicy&>(policies), dependency
        );
    }

    template<class TArg, class TDependency, class TPolicy, class... TCtor>
    static std::enable_if_t<has_compile_time<TPolicy>{}>
    call_impl_type(const TPolicy& policy, TDependency&& dependency) noexcept {
        call_impl_args<TArg, TDependency, TPolicy, TCtor...>(policy, dependency);
    }

    template<class TArg, class TDependency, class TPolicy, class... TCtor>
    static std::enable_if_t<!has_compile_time<TPolicy>{}>
    call_impl_type(const TPolicy& policy, TDependency&& dependency) noexcept {
        call_impl_args<TArg, TDependency, TPolicy, TCtor...>(policy, dependency);
    }

    template<class TArg, class TDependency, class TPolicy, class... TCtor>
    static std::enable_if_t<has_call_operator<TPolicy, TArg>{}>
    call_impl_args(const TPolicy& policy, TDependency&&) noexcept {
        (policy)(TArg{});
    }

    template<class TArg, class TDependency, class TPolicy, class... TCtor>
    static std::enable_if_t<has_call_operator<TPolicy, TArg, TDependency, TCtor...>{}>
    call_impl_args(const TPolicy& policy, TDependency&& dependency) noexcept {
        (policy)(TArg{}, dependency, aux::type<TCtor>{}...);
    }
};

}}} // boost::di::core

#endif

