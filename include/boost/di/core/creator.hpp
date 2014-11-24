//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CORE_CREATOR_HPP
#define BOOST_DI_CORE_CREATOR_HPP

#include "boost/di/aux_/memory.hpp"
#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/aux_/utility.hpp"
#include "boost/di/core/any_type.hpp"
#include "boost/di/core/binder.hpp"
#include "boost/di/core/pool.hpp"
#include "boost/di/wrappers/universal.hpp"
#include "boost/di/type_traits/ctor_traits.hpp"

namespace boost {
namespace di {
namespace core {

BOOST_MPL_HAS_XXX_TRAIT_DEF(any)
BOOST_MPL_HAS_XXX_TRAIT_DEF(create2)
BOOST_MPL_HAS_XXX_TRAIT_DEF(create3)

template<typename TInjector>
class creator {
    template<typename TDependency>
    using scope_create = typename aux::function_traits<
        decltype(&TDependency::create3)
    >::args;

    template<typename T, typename TDependency>
    struct data_visitor {
        using type = T;
        using dependency = TDependency;
        //using binder = binder_t;
    };

public:
    explicit creator(TInjector& injector)
        : injector_(injector)
    { }

    template<
        typename T
      , typename TParent
      , typename TProvider
      , typename TDeps
      , typename TRefs
      , typename TVisitor
      , typename TPolicies
    >
    aux::enable_if_t<
        has_any<T>::value
      , any_type<TParent, creator, TProvider, TDeps, TRefs, TVisitor, TPolicies>
    >
    create(const TProvider& provider
         , TDeps& deps
         , TRefs& refs
         , const TVisitor& visitor
         , const TPolicies& policies) {
        return any_type<TParent, creator, TProvider, TDeps, TRefs, TVisitor, TPolicies>(
            *this, provider, deps, refs, visitor, policies
        );
    }

    template<
        typename T
      , typename TParent
      , typename TProvider
      , typename TDeps
      , typename TRefs
      , typename TVisitor
      , typename TPolicies
    >
    aux::enable_if_t<!has_any<T>::value, wrappers::universal<T>>
    create(const TProvider& provider, TDeps& deps, TRefs& refs, const TVisitor& visitor, const TPolicies& policies) {
		using dependency_type = typename binder<TDeps>::template resolve<T>;
		//std::cout << boost::units::detail::demangle(typeid(dependency_type).name()) << std::endl;
        //assert_policies<data_visitor<T, dependency_type>>(policies);
        (visitor)(data_visitor<T, dependency_type>());
        return create_impl<T, dependency_type>(provider, deps, refs, visitor, policies);
    }

private:
    template<
        typename T
      , typename TDependency
      , typename TProvider
      , typename TDeps
      , typename TRefs
      , typename TVisitor
      , typename TPolicies
    >
    //aux::enable_if_t<!size<scope_create<TDependency>>{} && !has_create2<TDependency>{}, wrappers::universal<T>>
    typename enable_if_c<
        !/*mpl::*/size<scope_create<TDependency> >::value &&
        !has_create2<TDependency>::value
      , wrappers::universal<T>
    >::type
    create_impl(const TProvider&, TDeps& deps, TRefs& refs, const TVisitor&, const TPolicies&) {
        return wrappers::universal<T>(refs, acquire<TDependency>(deps).create());
    }

     template<
            typename T
          , typename TDependency
          , typename TProvider
          , typename TDeps
          , typename TRefs
          , typename TVisitor
          , typename TPolicies
        >
    typename enable_if_c<
        /*mpl::*/size<scope_create<TDependency> >::value &&
        has_create2<TDependency>::value
      , wrappers::universal<T>
    >::type
    create_impl(const TProvider&, TDeps& deps, TRefs& refs, const TVisitor&, const TPolicies&) {
        return {refs, acquire<TDependency>(deps).create_(injector_)};
    }

    template<
        typename T
      , typename TDependency
      , typename TProvider
      , typename TDeps
      , typename TRefs
      , typename TVisitor
      , typename TPolicies
    >
    //aux::enable_if_t<size<scope_create<TDependency>>{}, wrappers::universal<T>>
        typename enable_if_c<
            /*mpl::*/size<scope_create<TDependency> >::value &&
            !has_create2<TDependency>::value
          , wrappers::universal<T> >::type
    create_impl(const TProvider& provider
              , TDeps& deps
              , TRefs& refs
              , const TVisitor& visitor
              , const TPolicies& policies) {

        using ctor_type =
            typename type_traits::ctor_traits<typename TDependency::given>::type;

        return {
            refs
          , acquire<TDependency>(deps).create(
                create_impl<T, TDependency>(
                    provider
                  , deps
                  , refs
                  , visitor
                  , policies
                  , ctor_type()
                )
            )
        };
    }

    template<
        typename T
      , typename TDependency
      , typename TProvider
      , typename TDeps
      , typename TRefs
      , typename TVisitor
      , typename TPolicies
      , typename... TArgs
    >
    auto create_impl(const TProvider& provider
              , TDeps& deps
              , TRefs& refs
              , const TVisitor& visitor
              , const TPolicies& policies
              , const type_list<TArgs...>&) {
		//TODO will be always called
        return provider.template get<TDependency>(create<TArgs, T>(provider, deps, refs, visitor, policies)...);
    }

    template<typename T, typename... TPolicies>
    static void assert_policies(const pool<type_list<TPolicies...>>& policies) {
        int dummy[]{0, (assert_policy<TPolicies, T>(policies), 0)...};
        (void)dummy;
    }

    template<typename TPolicy, typename T, typename TPolicies>
    static void assert_policy(const TPolicies& policies) {
        static_cast<const TPolicy&>(policies).template assert_policy<T>();
    }

    template<typename TDependency, typename TDeps>
    aux::enable_if_t<std::is_base_of<TDependency, TDeps>{}, TDependency&>
    acquire(TDeps& deps) {
        return static_cast<TDependency&>(deps);
    }

    template<typename TDependency, typename TDeps>
    aux::enable_if_t<!std::is_base_of<TDependency, TDeps>{}, TDependency>
    acquire(TDeps&) {
        return TDependency{};
    }

    TInjector& injector_;
};

} // namespace core
} // namespace di
} // namespace boost

#endif

