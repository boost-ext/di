//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CORE_INJECTOR_HPP
#define BOOST_DI_CORE_INJECTOR_HPP

#include <vector>

#include "boost/di/aux_/memory.hpp"
#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/aux_/utility.hpp"
#include "boost/di/core/pool.hpp"
#include "boost/di/core/any_type.hpp"
#include "boost/di/core/binder.hpp"
#include "boost/di/providers/nothrow.hpp"
#include "boost/di/wrappers/universal.hpp"
#include "boost/di/type_traits/ctor_traits.hpp"
#include "boost/di/scopes/deduce.hpp"

namespace boost {
namespace di {
namespace core {

BOOST_MPL_HAS_XXX_TRAIT_DEF(any)
BOOST_MPL_HAS_XXX_TRAIT_DEF(create2)
BOOST_MPL_HAS_XXX_TRAIT_DEF(create3)
BOOST_MPL_HAS_XXX_TRAIT_DEF(deps)

template<typename T>
using is_injector = bool_<has_deps<T>::value || aux::has_configure<T>::value>;

template<typename T, typename = void>
struct get_injector {
    using type = T;
};

template<typename T>
struct get_injector<T, aux::enable_if_t<aux::has_configure<T>{}>> {
    using type = typename aux::function_traits<decltype(&T::configure)>::result_type;
};

template<typename T, typename = void>
struct get_deps {
    using type = typename T::deps;
};

template<typename T>
struct get_deps<T, aux::enable_if_t<aux::has_configure<T>{}>> {
    using type = typename get_injector<T>::type::deps;
};

template<typename T, typename = typename is_type_list<T>::type, typename = typename is_injector<T>::type>
struct add_type_list;

template<typename T, typename TAny>
struct add_type_list<T, std::true_type, TAny> {
    using type = T;
};

template<typename T>
struct add_type_list<T, std::false_type, std::true_type> {
    using type = typename get_deps<T>::type;
};

template<typename T>
struct add_type_list<T, std::false_type, std::false_type> {
    using type = type_list<T>; //scope<scopes::deduce>::bind<T>;
};

template<typename... Ts>
using bindings_t = typename join<typename add_type_list<Ts>::type...>::type;

template<typename TDeps = type_list<>, typename TDefaultProvider = providers::nothrow>
class injector : public pool<TDeps> {
    using pool_t = pool<TDeps>;

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
    using deps = TDeps;

    // bind<...>, etc.  -> ignore
    // injector<...>    -> get all dependencies from the injector
    // dependency<...>  -> pass

    template<typename... TArgs>
    injector(const TArgs&... args)
        : pool_t(pool<type_list<TArgs...>>(pass_arg(args)...), init{})
    { }

    template<typename T, typename... TArgs>
    T create(const TArgs&... args) {
        pool<type_list<TArgs...>> visitors(args...);
        std::vector<aux::shared_ptr<void>> refs_;
        return create<T, none_t>(TDefaultProvider{}, refs_, visitors);
    }

    template<typename T, typename TProvider, typename... TArgs>
    T provide(const TProvider& provider, const TArgs&... args) {
        pool<type_list<TArgs...>> visitors(args...);
        std::vector<aux::shared_ptr<void>> refs_;
        return create<T, none_t>(provider, refs_, visitors);
    }

    template<typename TAction>
    void call(const TAction& action) {
        call_impl(static_cast<pool<deps>&>(*this), action, deps{});
    }

private:
    template<
        typename T
      , typename TParent
      , typename TProvider
      , typename TRefs
      , typename TVisitors
    >
    aux::enable_if_t<
        has_any<T>::value
      , any_type<TParent, injector, TProvider, TRefs, TVisitors>
    >
    create(const TProvider& provider
         , TRefs& refs
         , const TVisitors& visitors) {
        return any_type<TParent, injector, TProvider, TRefs, TVisitors>(
            *this, provider, refs, visitors
        );
    }

    template<
        typename T
      , typename TParent
      , typename TProvider
      , typename TRefs
      , typename TVisitors
    >
    aux::enable_if_t<!has_any<T>::value, wrappers::universal<T>>
    create(const TProvider& provider, TRefs& refs, const TVisitors& visitors) {
		using dependency_type = typename binder<pool_t>::template resolve<T>;
        return create_impl<T, dependency_type>(provider, refs, visitors);
    }

    template<
        typename T
      , typename TDependency
      , typename TProvider
      , typename TRefs
      , typename TVisitors
    >
    //aux::enable_if_t<!size<scope_create<TDependency>>{} && !has_create2<TDependency>{}, wrappers::universal<T>>
    typename enable_if_c<
        !/*mpl::*/size<scope_create<TDependency> >::value &&
        !has_create2<TDependency>::value
      , wrappers::universal<T>
    >::type
    create_impl(const TProvider&, TRefs& refs, const TVisitors&) {
        return wrappers::universal<T>(refs, acquire<TDependency>(static_cast<pool_t&>(*this)).create());
    }

     template<
            typename T
          , typename TDependency
          , typename TProvider
          , typename TRefs
          , typename TVisitors
        >
    typename enable_if_c<
        /*mpl::*/size<scope_create<TDependency> >::value &&
        has_create2<TDependency>::value
      , wrappers::universal<T>
    >::type
    create_impl(const TProvider&, TRefs& refs, const TVisitors&) {
        return {refs, acquire<TDependency>(static_cast<pool_t&>(*this)).create_(*this)};
    }

    template<
        typename T
      , typename TDependency
      , typename TProvider
      , typename TRefs
      , typename TVisitors
    >
    //aux::enable_if_t<size<scope_create<TDependency>>{}, wrappers::universal<T>>
        typename enable_if_c<
            /*mpl::*/size<scope_create<TDependency> >::value &&
            !has_create2<TDependency>::value
          , wrappers::universal<T> >::type
    create_impl(const TProvider& provider
              , TRefs& refs
              , const TVisitors& visitors) {

        using ctor_type =
            typename type_traits::ctor_traits<typename TDependency::given>::type;

        return {
            refs
          , acquire<TDependency>(static_cast<pool_t&>(*this)).create(
                create_impl<T, TDependency>(provider, refs, visitors, ctor_type())
            )
        };
    }

    template<
        typename T
      , typename TDependency
      , typename TProvider
      , typename TRefs
      , typename TVisitors
      , typename... TArgs
    >
    auto create_impl(const TProvider& provider
              , TRefs& refs
              , const TVisitors& visitors
              , const type_list<TArgs...>&) {
		//TODO will be always called
        return provider.template get<TDependency>(create<TArgs, T>(provider, refs, visitors)...);
    }

    template<typename T, typename... TVisitors>
    static void assert_visitors(const pool<type_list<TVisitors...>>& visitors) {
        int dummy[]{0, (assert_policy<TVisitors, T>(visitors), 0)...};
        (void)dummy;
    }

    template<typename TPolicy, typename T, typename TVisitors>
    static void assert_policy(const TVisitors& visitors) {
        static_cast<const TPolicy&>(visitors).template assert_policy<T>();
    }

    template<typename TDependency, typename TDeps_>
    aux::enable_if_t<std::is_base_of<TDependency, TDeps_>{}, TDependency&>
    acquire(TDeps_& deps) {
        return static_cast<TDependency&>(deps);
    }

    template<typename TDependency, typename TDeps_>
    aux::enable_if_t<!std::is_base_of<TDependency, TDeps_>{}, TDependency>
    acquire(TDeps_&) {
        return TDependency{};
    }

    template<typename TDeps_, typename TAction, typename... Ts>
    void call_impl(TDeps_& deps, const TAction& action, const type_list<Ts...>&) {
        int dummy[]{0, (call_impl<Ts>(deps, action), 0)...};
        (void)dummy;
    }

    template<typename T, typename TDeps_, typename TAction>
    aux::enable_if_t<aux::has_call<T>{}> call_impl(TDeps_& deps, const TAction& action) {
        static_cast<T&>(deps).call(action);
    }

    template<typename T, typename TDeps_, typename TAction>
    aux::enable_if_t<!aux::has_call<T>{}> call_impl(TDeps_&, const TAction&) { }

    template<typename T>
    aux::enable_if_t<!aux::has_configure<T>::value, const T&>
    pass_arg(const T& arg) const {
        return arg;
    }

    template<typename T>
    aux::enable_if_t<aux::has_configure<T>::value, typename get_injector<T>::type>
    pass_arg(const T& arg) const {
        return arg.configure();
    }
};

} // namespace core
} // namespace di
} // namespace boost

#endif

