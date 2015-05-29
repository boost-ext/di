//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CORE_INJECTOR_HPP
#define BOOST_DI_CORE_INJECTOR_HPP

#include "boost/di/aux_/compiler_specific.hpp"
#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/aux_/utility.hpp"
#include "boost/di/core/any_type.hpp"
#include "boost/di/core/binder.hpp"
#include "boost/di/core/copyable.hpp"
#include "boost/di/core/policy.hpp"
#include "boost/di/core/pool.hpp"
#include "boost/di/core/provider.hpp"
#include "boost/di/core/transform.hpp"
#include "boost/di/core/wrapper.hpp"
#include "boost/di/scopes/exposed.hpp"
#include "boost/di/type_traits/ctor_traits.hpp"
#include "boost/di/type_traits/config_traits.hpp"
#include "boost/di/type_traits/referable_traits.hpp"
#include "boost/di/concepts/creatable.hpp"
#include "boost/di/config.hpp"

namespace boost { namespace di { inline namespace v1 { namespace core {

BOOST_DI_HAS_METHOD(call, call);

struct from_injector { };
struct from_deps { };
struct init { };
struct with_error { };

template<
    class T
  , class TName
  , class TIsRoot
  , class TDeps
  , class TIgnore = std::false_type
> struct arg_wrapper {
    using type BOOST_DI_UNUSED = T;
    using name BOOST_DI_UNUSED = TName;
    using is_root BOOST_DI_UNUSED = TIsRoot;
    using ignore = TIgnore;

    template<class T_, class TName_, class TDefault_>
    using resolve =
        decltype(core::binder::resolve<T_, TName_, TDefault_>((TDeps*)0));
};

template<class T, class TInjector>
inline auto build(const TInjector& injector) noexcept {
    return T{injector};
}

template<class T>
inline decltype(auto) get_arg(const T& arg, const std::false_type&) noexcept {
    return arg;
}

template<class T>
inline decltype(auto) get_arg(const T& arg, const std::true_type&) noexcept {
    return arg.configure();
}

#define BOOST_DI_CORE_INJECTOR \
    template<class TConfig, class TPolicies = pool<>, class... TDeps> \
    class injector : public pool<transform_t<TDeps...>> \
                   , public type_traits::config_traits_t<TConfig, injector<TConfig, TPolicies, TDeps...>> \
                   , _

    #define BOOST_DI_TRY_POLICY \
           && policy::template try_call< \
                  arg_wrapper<type_traits::referable_traits_t<T, TDependency>, TName, TIsRoot, pool_t> \
                , TPolicies \
                , TDependency \
                , TCtor \
              >::value

    #define BOOST_DI_APPLY_POLICY \
        policy::template call<arg_wrapper<create_t, TName, TIsRoot, pool_t, std::true_type>>( \
            ((TConfig&)*this).policies(), dependency, ctor_t{} \
        );

    #include "boost/di/core/injector.ipp"

#undef BOOST_DI_CORE_INJECTOR
#undef BOOST_DI_TRY_POLICY
#undef BOOST_DI_APPLY_POLICY

#define BOOST_DI_CORE_INJECTOR \
    template<class TConfig, class... TDeps> \
    class injector<TConfig, pool<>, TDeps...> \
        : public pool<transform_t<TDeps...>> \
        , public type_traits::config_traits_t<TConfig, injector<TConfig, pool<>, TDeps...>> \
        , _

#define BOOST_DI_TRY_POLICY
#define BOOST_DI_APPLY_POLICY

    #include "boost/di/core/injector.ipp"

#undef BOOST_DI_CORE_INJECTOR
#undef BOOST_DI_TRY_POLICY
#undef BOOST_DI_APPLY_POLICY

}}}} // boost::di::v1::core

#endif

