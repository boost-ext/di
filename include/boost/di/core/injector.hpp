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

BOOST_DI_HAS_TYPE(any);
BOOST_DI_HAS_TYPE(create2);
BOOST_DI_HAS_TYPE(create3);
BOOST_DI_HAS_TYPE(deps);

template<class T>
using is_injector = bool_<has_deps<T>::value || aux::has_configure<T>::value>;

template<class T, class = void>
struct get_injector {
    using type = T;
};

template<class T>
struct get_injector<T, aux::enable_if_t<aux::has_configure<T>{}>> {
    using type = typename aux::function_traits<decltype(&T::configure)>::result_type;
};

template<class T, class = void>
struct get_deps {
    using type = class T::deps;
};

template<class T>
struct get_deps<T, aux::enable_if_t<aux::has_configure<T>{}>> {
    using type = typename get_injector<T>::type::deps;
};

template<class T, class = typename is_type_list<T>::type, class = typename is_injector<T>::type>
struct add_type_list;

template<class T, class TAny>
struct add_type_list<T, std::true_type, TAny> {
    using type = T;
};

template<class T>
struct add_type_list<T, std::false_type, std::true_type> {
    using type = typename get_deps<T>::type;
};

template<class T>
struct add_type_list<T, std::false_type, std::false_type> {
    using type = type_list<T>; //scope<scopes::deduce>::bind<T>;
};

template<class... Ts>
using bindings_t = typename join<typename add_type_list<Ts>::type...>::type;

template<class TDeps = type_list<>, class TDefaultProvider = providers::nothrow>
class injector : public pool<TDeps> {
    using pool_t = pool<TDeps>;

    template<class TDependency>
    using scope_create = typename aux::function_traits<decltype(&TDependency::create3)>::args;

    template<class T, class TDependency>
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

    template<class... TArgs>
    injector(const TArgs&... args)
        : pool_t(pool<type_list<TArgs...>>(pass_arg(args)...), init{})
    { }

    template<class T, class... TArgs>
    T create(const TArgs&... args) {
        pool<type_list<TArgs...>> visitors(args...);
        std::vector<aux::shared_ptr<void>> refs_;
        return create<T, none_t>(TDefaultProvider{}, refs_, visitors);
    }

    template<class T, class TProvider, class... TArgs>
    T provide(const TProvider& provider, const TArgs&... args) {
        pool<type_list<TArgs...>> visitors(args...);
        std::vector<aux::shared_ptr<void>> refs_;
        return create<T, none_t>(provider, refs_, visitors);
    }

    template<class TAction>
    void call(const TAction& action) {
        call_impl(static_cast<pool<deps>&>(*this), action, deps{});
    }

private:
    template<class T, class TParent, class TProvider, class TRefs, class TVisitors>
    aux::enable_if_t< has_any<T>::value , any_type<TParent, injector, TProvider, TRefs, TVisitors> >
    create(const TProvider& provider, TRefs& refs, const TVisitors& visitors) {
        return any_type<TParent, injector, TProvider, TRefs, TVisitors>(*this, provider, refs, visitors);
    }

    template<class T, class TParent, class TProvider, class TRefs, class TVisitors>
    aux::enable_if_t<!has_any<T>::value, wrappers::universal<T>>
    create(const TProvider& provider, TRefs& refs, const TVisitors& visitors) {
        return create_impl<T, typename binder<pool_t>::template resolve<T>>(provider, refs, visitors);
    }

    template<class T, class TDependency, class TProvider, class TRefs, class TVisitors>
    aux::enable_if_t<!size<scope_create<TDependency>>::value && !has_create2<TDependency>::value, wrappers::universal<T>>
    create_impl(const TProvider&, TRefs& refs, const TVisitors&) {
        return wrappers::universal<T>(refs, acquire<TDependency>(static_cast<pool_t&>(*this)).create());
    }

    template<class T, class TDependency, class TProvider, class TRefs, class TVisitors>
    aux::enable_if_t<size<scope_create<TDependency>>::value && has_create2<TDependency>::value, wrappers::universal<T>>
    create_impl(const TProvider&, TRefs& refs, const TVisitors&) {
        return {refs, acquire<TDependency>(static_cast<pool_t&>(*this)).create_(*this)};
    }

    template<class T, class TDependency, class TProvider, class TRefs, class TVisitors>
    aux::enable_if_t<size<scope_create<TDependency>>::value && !has_create2<TDependency>::value, wrappers::universal<T>>
    create_impl(const TProvider& provider, TRefs& refs, const TVisitors& visitors) {
        return { refs, acquire<TDependency>(static_cast<pool_t&>(*this)).create(
            create_impl<T, TDependency>(provider, refs, visitors, typename type_traits::ctor_traits<typename TDependency::given>::type{}))
        };
    }

    template<class T, class TDependency, class TProvider, class TRefs, class TVisitors, class... TArgs>
    auto create_impl(const TProvider& provider, TRefs& refs, const TVisitors& visitors, const type_list<TArgs...>&) {
        return provider.template get<TDependency>(create<TArgs, T>(provider, refs, visitors)...);
    }

    template<class T, class... TVisitors>
    static void assert_visitors(const pool<type_list<TVisitors...>>& visitors) {
        int dummy[]{0, (assert_policy<TVisitors, T>(visitors), 0)...};
        (void)dummy;
    }

    template<class TPolicy, class T, class TVisitors>
    static void assert_policy(const TVisitors& visitors) {
        static_cast<const TPolicy&>(visitors).template assert_policy<T>();
    }

    template<class TDependency, class TDeps_>
    aux::enable_if_t<std::is_base_of<TDependency, TDeps_>{}, TDependency&>
    acquire(TDeps_& deps) {
        return static_cast<TDependency&>(deps);
    }

    template<class TDependency, class TDeps_>
    aux::enable_if_t<!std::is_base_of<TDependency, TDeps_>{}, TDependency>
    acquire(TDeps_&) {
        return TDependency{};
    }

    template<class TDeps_, class TAction, class... Ts>
    void call_impl(TDeps_& deps, const TAction& action, const type_list<Ts...>&) {
        int dummy[]{0, (call_impl<Ts>(deps, action), 0)...};
        (void)dummy;
    }

    template<class T, class TDeps_, class TAction>
    aux::enable_if_t<aux::has_call<T>{}> call_impl(TDeps_& deps, const TAction& action) {
        static_cast<T&>(deps).call(action);
    }

    template<class T, class TDeps_, class TAction>
    aux::enable_if_t<!aux::has_call<T>{}> call_impl(TDeps_&, const TAction&) { }

    template<class T>
    aux::enable_if_t<!aux::has_configure<T>::value, const T&>
    pass_arg(const T& arg) const {
        return arg;
    }

    template<class T>
    aux::enable_if_t<aux::has_configure<T>::value, typename get_injector<T>::type>
    pass_arg(const T& arg) const {
        return arg.configure();
    }
};

} // namespace core
} // namespace di
} // namespace boost

#endif

