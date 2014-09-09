//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CORE_CREATOR_HPP
#define BOOST_DI_CORE_CREATOR_HPP

#include <unordered_map>
#include "boost/di/aux_/config.hpp"
#include "boost/di/aux_/memory.hpp"
#include "boost/di/aux_/mpl.hpp"
#include "boost/di/aux_/utility.hpp"
#include "boost/di/core/any_type.hpp"
#include "boost/di/core/binder.hpp"
#include "boost/di/core/pool.hpp"
#include "boost/di/type_traits/ctor_traits.hpp"
#include "boost/di/type_traits/function_traits.hpp"
#include "boost/di/wrappers/universal.hpp"

namespace boost {
namespace di {
namespace core {

BOOST_DI_HAS_MEMBER_TYPE(any);

template<typename TDeps_>
class creator {
    using binder_t = binder<TDeps_>;
    using scopes_type = std::unordered_map<int, aux::shared_ptr<void>>;

    template<typename TDependency>
    using scope_create = typename type_traits::function_traits<
        decltype(&TDependency::create)
    >::type;

    template<typename T, typename TDependency>
    struct data_visitor {
        using type = T;
        using dependency = TDependency;
        using binder = binder_t;
    };

public:
    template<
        typename T
      , typename TParent
      , typename TCallStack
      , typename TProvider
      , typename TDeps
      , typename TRefs
      , typename TVisitor
      , typename TPolicies
    >
    typename std::enable_if<
        has_any<T>::value
      , any_type<
            TParent
          , TCallStack
          , creator
          , TProvider
          , TDeps
          , TRefs
          , TVisitor
          , TPolicies
        >
    >::type
    create(const TProvider& provider
         , TDeps& deps
         , TRefs& refs
         , const TVisitor& visitor
         , const TPolicies& policies) {
        return any_type<
            TParent
          , TCallStack
          , creator
          , TProvider
          , TDeps
          , TRefs
          , TVisitor
          , TPolicies
        >(*this, provider, deps, refs, visitor, policies);
    }

    template<
        typename T
      , typename TParent
      , typename TCallStack
      , typename TProvider
      , typename TDeps
      , typename TRefs
      , typename TVisitor
      , typename TPolicies
    >
    typename std::enable_if<!has_any<T>::value, wrappers::universal<T>>::type
    create(const TProvider& provider
         , TDeps& deps
         , TRefs& refs
         , const TVisitor& visitor
         , const TPolicies& policies) {
        using call_stack = typename std::conditional<
            std::is_same<TParent, none_t>::value
          , TCallStack
          , typename add<TCallStack, TParent>::type
        >::type;
        using eval_type = typename binder_t::template eval<T, call_stack>::type;
        using dependency_type = typename binder_t::template resolve<T, call_stack>::type;
        using propagate_call_stack = typename std::conditional<eval_type::value, call_stack, TCallStack>::type;

        assert_policies<data_visitor<T, dependency_type>>(policies);
        (visitor)(data_visitor<T, dependency_type>());

        return create_impl<T, dependency_type, propagate_call_stack>(
            provider, deps, refs, visitor, policies
        );
    }

private:
    template<
        typename T
      , typename TDependency
      , typename TCallStack
      , typename TProvider
      , typename TDeps
      , typename TRefs
      , typename TVisitor
      , typename TPolicies
    >
    typename std::enable_if<
        !size<scope_create<TDependency>>::value
      , wrappers::universal<T>
    >::type
    create_impl(const TProvider&
              , TDeps& deps
              , TRefs& refs
              , const TVisitor&
              , const TPolicies&) {
        return wrappers::universal<T>(
            refs, acquire<TDependency>(deps).create()
        );
    }

    template<
        typename T
      , typename TDependency
      , typename TCallStack
      , typename TProvider
      , typename TDeps
      , typename TRefs
      , typename TVisitor
      , typename TPolicies
    >
    typename std::enable_if<
        size<scope_create<TDependency>>::value
      , wrappers::universal<T>>::type
    create_impl(const TProvider& provider
              , TDeps& deps
              , TRefs& refs
              , const TVisitor& visitor
              , const TPolicies& policies) {

        using ctor_type =
            typename type_traits::ctor_traits<typename TDependency::given>::type;

        return wrappers::universal<T>(
            refs
          , acquire<TDependency>(deps).create(
                create_impl<T, TDependency, TCallStack>(
                    provider
                  , deps
                  , refs
                  , visitor
                  , policies
                  , ctor_type()
                )
            )
        );
    }

    template<
        typename T
      , typename TDependency
      , typename TCallStack
      , typename TProvider
      , typename TDeps
      , typename TRefs
      , typename TVisitor
      , typename TPolicies
      , typename... TArgs
    >
    typename TDependency::expected*
    create_impl(const TProvider& provider
              , TDeps& deps
              , TRefs& refs
              , const TVisitor& visitor
              , const TPolicies& policies
              , const type_list<TArgs...>&) {
        (void)provider; (void)deps; (void)refs; (void)visitor; (void)policies;
        return provider.template
            get<typename TDependency::expected, typename TDependency::given>(
                create<TArgs, T, TCallStack>(provider, deps, refs, visitor, policies)...
        );
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
    typename std::enable_if<std::is_base_of<TDependency, TDeps>::value, TDependency&>::type
    acquire(TDeps& deps) {
        return static_cast<TDependency&>(deps);
    }

    template<typename TDependency, typename TDeps>
    typename std::enable_if<!std::is_base_of<TDependency, TDeps>::value, TDependency&>::type
    acquire(TDeps&) {
        auto it = scopes_.find(aux::type_id<TDependency>());
        if (it != scopes_.end()) {
            return *static_cast<TDependency*>(it->second.get());
        }

        aux::shared_ptr<TDependency> dependency(new TDependency());
        scopes_.insert(std::make_pair(aux::type_id<TDependency>(), dependency));
        return *dependency;
    }

    scopes_type scopes_;
};

} // namespace core
} // namespace di
} // namespace boost

#endif

